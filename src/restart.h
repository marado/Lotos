/* vi: set ts=4 sw=4 ai: */
/*
 * restart.h
 *
 *   Lotos v1.2.2  : (c) 1999-2002 Pavol Hluchy (Lopo)
 *   last update   : 16.5.2002
 *   email         : lopo@losys.sk
 *   homepage      : lopo.losys.sk
 *   Lotos homepage: lotos.losys.sk
 */

#ifndef __RESTART_H__
#define __RESTART_H__ 1

extern UR_OBJECT user_first;
extern RM_OBJECT room_first;
#ifdef NETLINKS
	extern NL_OBJECT nl_first;
#endif
extern PL_OBJECT plugin_first;
extern SYS_OBJECT amsys;
extern SYSPP_OBJECT syspp;

extern char progname[], confile[];
extern int listen_sock[], port[];


extern char *syserror;
extern char *default_jail;
extern char *clone_desc;
extern char *restart_prompt, *restart_ok;

#endif /* __RESTART_H__ */

