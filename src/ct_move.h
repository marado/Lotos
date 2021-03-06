/* vi: set ts=4 sw=4 ai: */
/*
 * ct_move.h
 *
 *   Lotos v1.2.3  : (c) 1999-2003 Pavol Hluchy (Lopo)
 *   last update   : 30.1.2003
 *   email         : lotos@losys.sk
 *   homepage      : lotos.losys.sk
 */

#ifndef __CT_MOVE_H__
#define __CT_MOVE_H__ 1

extern RM_OBJECT room_first;
extern SYS_OBJECT amsys;
extern SYSPP_OBJECT syspp;

extern char text[];
extern char word[MAX_WORDS][WORD_LEN+1];
extern int no_prompt;
extern int word_count;

extern char *nosuchroom, *notloggedon, *nosuchuser;
extern char *invisleave, *invisname;

extern char *crypt_salt;

extern char *default_warp;
extern char *default_personal_room_desc;

extern struct {
	char *name,*alias;
	int level,function;
	} command_table[];

extern char *restrict_string;


/* prompts */
extern char *already_in_room;
extern char *move_prompt_user, *user_room_move_prompt;

#endif /* __CT_MOVE_H__ */

