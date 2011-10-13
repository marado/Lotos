/*****************************************************************************
          Funkcie OS Star v1.0.0b na pracu s pocitadlami a zoznamami
            Copyright (C) Pavol Hluchy - posledny update: 28.3.2000
          osstar@star.sjf.stuba.sk  |  http://star.sjf.stuba.sk/osstar
 *****************************************************************************/

#include <stdio.h>
#include <dirent.h>
#include <time.h>
#include <string.h>

#include "define.h"
#include "ur_obj.h"
#include "syspp_obj.h"
#include "poc_zoz.h"

void load_counters(void)
{
	FILE *fp;
	char filename[300];
	int err=0, i, tmp=0;

	printf("Nacitavam pocitadla ");
	sprintf(filename, "%s/%s/%s/%s",
		ROOTDIR, DATAFILES, COUNTFILES, TCOUNTER);
	if ((fp=fopen(filename, "r"))==NULL) {
		write_syslog(ERRLOG, 0, "Nemozem otvorit fajl s tcountrom, nulujem\n");
		for (i=0; i<4; i++) {
			syspp->tcounter[i]=0;
			printf(".");
			}
		err=1;
		}
	else {
		fscanf(fp, "%ld %ld %ld %d",
			&syspp->tcounter[0], &syspp->tcounter[1],
			&syspp->tcounter[2], &syspp->tcounter[3]
			);
		fclose(fp);
		printf("....");
		}
	for (i=0; i<3; i++)
		tmp+=syspp->tcounter[i];
	if (tmp!=syspp->tcounter[3]) {
		err=1;
		write_syslog(ERRLOG, 0, "Chybne ulozeny sucet conutrov, prepocitavam novy\n");
		syspp->tcounter[3]=tmp;
		}
	for (i=0; i<4; i++) {
		syspp->bcounter[i]=0;
		printf(".");
	}
	for (i=0; i<4; i++) {
		syspp->acounter[i]=0;
		printf(".");
		}

	sprintf(filename, "%s/%s/%s/%s",
		ROOTDIR, DATAFILES, COUNTFILES, MCOUNTER);
	if ((fp=fopen(filename, "r"))==NULL) {
		write_syslog(ERRLOG, 0, "Nemozem otvorit fajl s mcountrom, nulujem\n");
		for (i=0; i<4; i++) {
			syspp->mcounter[i]=0;
			printf(".");
			}
		err=1;
		}
	else {
		fscanf(fp, "%ld %ld %ld %d",
			&syspp->mcounter[0], &syspp->mcounter[1],
			&syspp->mcounter[2], &syspp->mcounter[3]
			);
		fclose(fp);
		printf("....");
		}

	printf(" OK");
	if (!err) printf("\n");
	else printf(" kukni \033[1m\033[31merrlog\033[0m\n");
}


void save_counters(void)
{
	FILE *fp;
	int i;
	char filename[300];

	sprintf(filename, "%s/%s/%s/%s",
		ROOTDIR, DATAFILES, COUNTFILES, TCOUNTER);
	if ((fp=fopen(filename, "w"))==NULL) {
		write_syslog(ERRLOG, "Nemozem otvorit tcounter fajl pre zapis v save_counters()\n");
		return;
		}
	for (i=0; i<4; i++)
		fprintf(fp, "%ld ", syspp->tcounter[i]);
	fprintf(fp, "\n");
	fclose(fp);

	sprintf(filename, "%s/%s/%s/%s",
		ROOTDIR, DATAFILES, COUNTFILES, MCOUNTER);
	if ((fp=fopen(filename, "w"))==NULL) {
		write_syslog(ERRLOG, "Nemozem otvorit mcounter fajl pre zapis v save_counters()\n");
		return;
		}
	for (i=0; i<4; i++)
		fprintf(fp, "%ld ", syspp->mcounter[i]);
	fprintf(fp, "\n");
	fclose(fp);
}


void show_counters(UR_OBJECT user)
{
	write_user(user, "Aktualny stav pocitadiel:\n\n");
	write_user(user, "                   Loginy                      Useri\n");
	write_user(user, "Pohlavie   |   Total        Boot     |  Maximum     Aktual\n");
	write_user(user, "-----------+-------------------------+--------------------\n");
	vwrite_user(user,"Muzi       |  %6ld      %6ld     |  %6ld      %6ld\n",
		syspp->tcounter[1], syspp->bcounter[1], syspp->mcounter[1], syspp->acounter[1]);
	vwrite_user(user,"Zeny       |  %6ld      %6ld     |  %6ld      %6ld\n",
		syspp->tcounter[2], syspp->bcounter[2], syspp->mcounter[2], syspp->acounter[2]);
	vwrite_user(user,"Neurcite   |  %6ld      %6ld     |  %6ld      %6ld\n",
		syspp->tcounter[0], syspp->bcounter[0], syspp->mcounter[0], syspp->acounter[0]);
	write_user(user, "-----------+-------------------------+--------------------\n");
	vwrite_user(user,"Spolu      |  %6ld      %6ld     |  %6ld      %6ld\n",
		syspp->tcounter[3], syspp->bcounter[3], syspp->mcounter[3], syspp->acounter[3]);
}


int count_musers(UR_OBJECT user, char *inpstr)
{
	int i=0, count=0;
	int lastspace=0, lastcomma=0, gotchar=0;
	int point=0, point2=0;
	char multiliststr[ARR_SIZE];
	char *strbck, *p=inpstr;

	strbck=strdup(inpstr);
	reset_murlist(user);

	for (i=0; i<strlen(inpstr); ++i) {
		if (inpstr[i]==' ') {
			if (lastspace && !gotchar) {
				point++;
				point2++;
				continue;
				}
			if (!gotchar) {
				point++;
				point2++;
				}
			lastspace=1;
			continue;
			} /* end of if space */
		else if (inpstr[i]=='+') {
			if (!gotchar) {
				lastcomma=1;
				point++;
				point2++;
				continue;
				}
			else {
				if (count <= MAX_MUSERS-1) {
					if (((point2-1)-point)<=USER_NAME_LEN)
						midcpy(inpstr, user->murlist[count],point,point2-1);
					count++;
					}
				point=i+1;
				point2=point;
				gotchar=0;
				lastcomma=1;
				continue;
				}
			} /* end of if comma */
		if ((inpstr[i-1]==' ') && (gotchar)) {
			if (count <= MAX_MUSERS-1) {
				if (((point2-1)-point)<=USER_NAME_LEN)
					midcpy(inpstr,user->murlist[count],point,point2-1);
				count++;  
				}
			break;
			}
		gotchar=1;
		lastcomma=0;
		lastspace=0;
		point2++;
		} /* end of for */
	midcpy(inpstr,multiliststr,i,ARR_SIZE);

	if (!strlen(multiliststr)) {
		/* no message string, copy last user */
		if ((point2-point)<=USER_NAME_LEN)
		midcpy(inpstr,user->murlist[count],point,point2);
		count++;
		inpstr[0]='\0';
		}
	else {
		strcpy(inpstr,multiliststr);
		multiliststr[0]=0;
		}

	i=0;
	point=0;
	point2=0;
	gotchar=0;
	inpstr=p;
	strcpy(inpstr, strbck);
	free(strbck);
	return count;
}


void list_txt_files(UR_OBJECT user)
{
	DIR *dirp;
	FILE *ifp, *ofp;
	struct dirent *dp;
	char dirname[200], filename[250], filename2[250];
	int cnt, tcnt;

	cnt=tcnt=0;
	if (!user) printf("Vytvaram zoznam textovych suborov ... ");
	else write_user(user, "Vytvaram zoznam textovych suborov ");
	sprintf(dirname, "%s/%s/%s", ROOTDIR, DATAFILES, TEXTFILES);

	if (!(dirp=opendir(dirname))) {
		if (!user) {
			fprintf(stderr, "\nOS Star: Directory open failure in list_txt_files().\n");
			boot_exit(101);
			}
		else {
			write_user(user, "\nOS Star: Directory open failure in list_txt_files().\n");
			return;
			}
		}
	sprintf(filename, "%s/%s/showfiles.tmp", ROOTDIR, TEMPFILES);
	if ((ofp=fopen(filename, "w"))==NULL) {
		(void) closedir(dirp);
		if (!user) {
			fprintf(stderr, "\nOS Star: Nemozem vytvorit tempfajl v list_txt_files().\n");
			boot_exit(102);
			}
		else {
			write_user(user, "\nOS Star: Nemozem vytvorit tempfajl v list_txt_files().\n");
			return;
			}
		}
	fprintf(ofp, "\n+----- ~FG~OLFiles~RS ----------------------------------------------------------------+\n\n");
	fprintf(ofp, "Precitaj si nasledujuce subory pre dalsie informacie o talkri.\n\n");

	while((dp=readdir(dirp))!=NULL) {
		if (!strcmp(dp->d_name, ".")
		    || !strcmp(dp->d_name, "..")
		    || !strcmp(dp->d_name, ADMINFILES)
		    ) continue;
		sprintf(filename, "%s/%s", dirname, dp->d_name);
		if ((ifp=fopen(filename, "r"))==NULL) {
			(void) closedir(dirp);
			if (!user) {
				fprintf(stderr, "\nOS Star: Nemozem otvorit subor na citanie v list_txt_files().\n");
				boot_exit(103);
				}
			else {
				write_user(user, "\nOS Star: Nemozem otvorit subor na citanie v list_txt_files().\n");
				return;
				}
			}
		fgets(text, ARR_SIZE-1, ifp);
		fclose(ifp);
		if (user) write_user(user, ".");
		fprintf(ofp, "* ~OL%-8.8s~RS - %-67.67s\n", dp->d_name, text);
		cnt++;
		}
	(void) closedir(dirp);
	if (!cnt) fprintf(ofp, "Momentalne neni su ziadne subory\n");
	fprintf(ofp, "+----------------------------------------------------------------------------+\n\n");
	fclose(ofp);
	sprintf(filename, "%s/%s/showfiles.tmp", ROOTDIR, TEMPFILES);
	sprintf(filename2, "%s/%s/%s/%s", ROOTDIR, DATAFILES, MISCFILES, SHOWFILES);
	unlink(filename2);
	rename(filename, filename2);
	tcnt=cnt;

	cnt=0;
	sprintf(dirname, "%s/%s/%s/%s", ROOTDIR, DATAFILES, TEXTFILES, ADMINFILES);

	if (!(dirp=opendir(dirname))) {
		if (!user) {
			fprintf(stderr, "\nOS Star: Directory open failure in list_txt_files().\n");
			boot_exit(101);
			}
		else {
			write_user(user, "\nOS Star: Directory open failure in list_txt_files().\n");
			return;
			}
		}
	sprintf(filename, "%s/%s/showfiles.tmp", ROOTDIR, TEMPFILES);
	if ((ofp=fopen(filename, "w"))==NULL) {
		(void) closedir(dirp);
		if (!user) {
			fprintf(stderr, "\nOS Star: Nemozem vytvorit tempfajl v list_txt_files().\n");
			boot_exit(102);
			}
		else {
			write_user(user, "\nOS Star: Nemozem vytvorit tempfajl v list_txt_files().\n");
			return;
			}
		}
	fprintf(ofp, "\n+----- ~FG~OLAdmin Files~RS ----------------------------------------------------------+\n\n");
	fprintf(ofp, "Precitaj si nasledujuce subory pre dalsie admin informacie o talkri.\n\n");

	while((dp=readdir(dirp))!=NULL) {
		if (!strcmp(dp->d_name, ".")
		    || !strcmp(dp->d_name, "..")
		    ) continue;
		sprintf(filename, "%s/%s", dirname, dp->d_name);
		if ((ifp=fopen(filename, "r"))==NULL) {
			(void) closedir(dirp);
			fclose(ofp);
			if (!user) {
				fprintf(stderr, "\nOS Star: Nemozem otvorit subor na citanie v list_txt_files().\n");
				boot_exit(103);
				}
			else {
				write_user(user, "\nOS Star: Nemozem otvorit subor na citanie v list_txt_files().\n");
				return;
				}
			}
		fgets(text, ARR_SIZE-1, ifp);
		fclose(ifp);
		if (user) write_user(user, ".");
		fprintf(ofp, "* ~OL%-8.8s~RS - %-67.67s\n", dp->d_name, text);
		cnt++;
		}
	(void) closedir(dirp);
	if (!cnt) fprintf(ofp, "Momentalne neni su ziadne subory\n");
	fprintf(ofp, "+----------------------------------------------------------------------------+\n\n");
	fclose(ofp);
	sprintf(filename, "%s/%s/showfiles.tmp", ROOTDIR, TEMPFILES);
	sprintf(filename2, "%s/%s/%s/%s", ROOTDIR, DATAFILES, MISCFILES, SHOWAFILES);
	unlink(filename2);
	rename(filename, filename2);
	tcnt+=cnt;
	if (!user) printf(" spolu %d\n", tcnt);
	else vwrite_user(user, " spolu %d\n", tcnt);
}


void list_pic_files(UR_OBJECT user)
{
	DIR *dirp;
	FILE *ifp, *ofp;
	struct dirent *dp;
	char dirname[200], filename[250], filename2[250];
	char fntname[50];
	int cnt=0, cl, pc;

	if (!user) printf("Vytvaram zoznam obrazkovych suborov ... ");
	else write_user(user, "Vytvaram zoznam obrazkovych suborov ");
	sprintf(dirname, "%s/%s/%s", ROOTDIR, DATAFILES, PICTFILES);

	if (!(dirp=opendir(dirname))) {
		if (!user) {
			fprintf(stderr, "\nOS Star: Directory open failure in list_pic_files().\n");
			boot_exit(101);
			}
		else {
			write_user(user, "\nOS Star: Directory open failure in list_pic_files().\n");
			return;
			}
		}
	sprintf(filename, "%s/%s/pictfiles.tmp", ROOTDIR, TEMPFILES);
	if ((ofp=fopen(filename, "w"))==NULL) {
		(void) closedir(dirp);
		if (!user) {
			fprintf(stderr, "\nOS Star: Nemozem vytvorit tempfajl v list_pic_files().\n");
			boot_exit(102);
			}
		else {
			write_user(user, "\nOS Star: Nemozem vytvorit tempfajl v list_pic_files().\n");
			return;
			}
		}
	fprintf(ofp, "\n+----- ~FG~OLZoznam obrazkov~RS ------------------------------------------------------+\n\n");

	pc=0;
	while((dp=readdir(dirp))!=NULL) {
		if (!strcmp(dp->d_name, ".")
		    || !strcmp(dp->d_name, "..")
		    || !strstr(dp->d_name, ".pic")
		    || !strncmp(dp->d_name, ".pic", 4)
		    ) continue;
		sprintf(filename, "%s/%s", dirname, dp->d_name);
		if ((cl=count_lines(filename))==0) continue;
		strcpy(fntname, dp->d_name);
		fntname[strlen(fntname)-4]='\0';
		if (cl>23) fprintf(ofp, "~OL~FR%-19.19s~RS", fntname);
		else fprintf(ofp, "~OL%-19.19s~RS", fntname);
		pc++;
		if (pc==4) {
			fprintf(ofp, "\n");
			pc=0;
			}
		if (user) write_user(user, ".");
		cnt++;
		}
	(void) closedir(dirp);
	if (!cnt) fprintf(ofp, "Momentalne neni su ziadne obrazky\n");
	fprintf(ofp, "\n+----------------------------------------------------------------------------+\n\n");
	fclose(ofp);
	sprintf(filename, "%s/%s/pictfiles.tmp", ROOTDIR, TEMPFILES);
	sprintf(filename2, "%s/%s/%s/pictlist", ROOTDIR, DATAFILES, MISCFILES);
	unlink(filename2);
	rename(filename, filename2);
	if (!user) printf(" spolu %d\n", cnt);
	else vwrite_user(user, " spolu %d\n", cnt);
}


void list_fnt_files(UR_OBJECT user)
{
	DIR *dirp;
	FILE *ifp, *ofp;
	struct dirent *dp;
	char dirname[200], filename[250], filename2[250];
	char name[ARR_SIZE];
	int cnt=0, cl, pc;

	if (!user) printf("Vytvaram zoznam fontov ... ");
	else write_user(user, "Vytvaram zoznam fontov ");
	sprintf(dirname, "%s/%s/%s", ROOTDIR, DATAFILES, FIGLET_FONTS);

	if (!(dirp=opendir(dirname))) {
		if (!user) {
			fprintf(stderr, "\nOS Star: Directory open failure in list_fnt_files().\n");
			boot_exit(101);
			}
		else {
			write_user(user, "\nOS Star: Directory open failure in list_fnt_files().\n");
			return;
			}
		}
	sprintf(filename, "%s/%s/fntfiles.tmp", ROOTDIR, TEMPFILES);
	if ((ofp=fopen(filename, "w"))==NULL) {
		(void) closedir(dirp);
		if (!user) {
			fprintf(stderr, "\nOS Star: Nemozem vytvorit tempfajl v list_fnt_files().\n");
			boot_exit(102);
			}
		else {
			write_user(user, "\nOS Star: Nemozem vytvorit tempfajl v list_fnt_files().\n");
			return;
			}
		}
	fprintf(ofp, "\n+----- ~FG~OLZoznam fontov~RS ------------------------------------------------------+\n\n");

	pc=0;
	while((dp=readdir(dirp))!=NULL) {
		if (!strcmp(dp->d_name, ".")
		    || !strcmp(dp->d_name, "..")
		    || !strstr(dp->d_name, ".flf")
		    || !strncmp(dp->d_name, ".flf", 4)
		    ) continue;
		strcpy(name, dp->d_name);
		name[strlen(name)-4]='\0';
		fprintf(ofp, "~OL%-19.19s~RS", name);
		pc++;
		if (pc==4) {
			fprintf(ofp, "\n");
			pc=0;
			}
		if (user) write_user(user, ".");
		cnt++;
		}
	(void) closedir(dirp);
	if (!cnt) fprintf(ofp, "Momentalne neni su ziadne fonty\n");
	fprintf(ofp, "\n+----------------------------------------------------------------------------+\n\n");
	fclose(ofp);
	sprintf(filename, "%s/%s/fntfiles.tmp", ROOTDIR, TEMPFILES);
	sprintf(filename2, "%s/%s/%s/fontslist", ROOTDIR, DATAFILES, MISCFILES);
	unlink(filename2);
	rename(filename, filename2);
	if (!user) printf(" spolu %d\n", cnt);
	else vwrite_user(user, " spolu %d\n", cnt);
}


void list_kill_msgs(UR_OBJECT user)
{
	DIR *dirp;
	FILE *ifp, *ofp;
	struct dirent *dp;
	char dirname[200], filename[250], filename2[250];
	char *pp, line[82];
	int cnt=0, cl, pc;

	if (!user) printf("Vytvaram zoznam 'kill hlasok' ... ");
	else write_user(user, "Vytvaram zoznam 'kill hlasok' ");
	sprintf(dirname, "%s/%s/%s", ROOTDIR, DATAFILES, KILLMSGS);

	if (!(dirp=opendir(dirname))) {
		if (!user) {
			fprintf(stderr, "\nOS Star: Directory open failure in list_kill_msgs().\n");
			boot_exit(101);
			}
		else {
			write_user(user, "\nOS Star: Directory open failure in list_kill_msgs().\n");
			return;
			}
		}
	sprintf(filename, "%s/%s/killmsgs.tmp", ROOTDIR, TEMPFILES);
	if ((ofp=fopen(filename, "w"))==NULL) {
		(void) closedir(dirp);
		if (!user) {
			fprintf(stderr, "\nOS Star: Nemozem vytvorit tempfajl v list_kill_msgs().\n");
			boot_exit(102);
			}
		else {
			write_user(user, "\nOS Star: Nemozem vytvorit tempfajl v list_kill_msgs().\n");
			return;
			}
		}
	fprintf(ofp, "\n+----- ~FG~OLZoznam kill hlasok~RS -------------------------------------------------+\n\n");

	pc=0;
	cnt=0;
	while((dp=readdir(dirp))!=NULL) {
		if (!strcmp(dp->d_name, ".")
		    || !strcmp(dp->d_name, "..")
		    || strncmp(dp->d_name, "kill.", 5)
		    ) continue;
		sprintf(filename, "%s/%s", dirname, dp->d_name);
		if (!(ifp=fopen(filename, "r"))) continue;
		else {
			fgets(line, 81, ifp);
			pp=strchr(line, '\n');
			if (pp) pp[0]='\0';
			fclose(ifp);
			}
		pp=strchr(dp->d_name, '.');
		pc=atoi(++pp);
		fprintf(ofp, "%2d - %-75.75s\n", pc, line);
		if (pc>syspp->kill_msgs) syspp->kill_msgs=pc;
		if (user) write_user(user, ".");
		cnt++;
		}
	(void) closedir(dirp);
	if (!cnt) fprintf(ofp, "Momentalne neni su ziadne 'kill hlasky'\n");
	fprintf(ofp, "\n+----------------------------------------------------------------------------+\n\n");
	fclose(ofp);
	sprintf(filename, "%s/%s/killmsgs.tmp", ROOTDIR, TEMPFILES);
	sprintf(filename2, "%s/%s/%s/killmsgs", ROOTDIR, DATAFILES, MISCFILES);
	unlink(filename2);
	rename(filename, filename2);
	if (!user) printf(" spolu %d\n", cnt);
	else vwrite_user(user, " spolu %d\n", cnt);
}

