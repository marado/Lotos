/* vi: set ts=4 sw=4 ai: */
/*
 * obj_nl.h
 *
 *   Lotos v1.2.3  : (c) 1999-2003 Pavol Hluchy (Lopo)
 *   last update   : 30.1.2003
 *   email         : lotos@losys.sk
 *   homepage      : lotos.losys.sk
 */

#ifndef __OBJ_NL_H__
#define __OBJ_NL_H__ 1

#include "define.h"

/* Structure for net links, ie server initiates them */
struct netlink_struct {
	char service[SERV_NAME_LEN+1];
	char site[SITE_NAME_LEN+1];
	char verification[VERIFY_LEN+1];
	char buffer[ARR_SIZE*2];
	char mail_to[WORD_LEN+1];
	char mail_from[WORD_LEN+1];
	FILE *mailfile;
	time_t last_recvd; 
	int port,socket,type,connected;
	int stage,lastcom,allow,warned,keepalive_cnt;
	int ver_major,ver_minor,ver_patch;
	struct user_struct *mesg_user;
	struct room_struct *connect_room;
	struct netlink_struct *prev,*next;
	};
typedef struct netlink_struct *NL_OBJECT;

#endif /* __OBJ_NL_H__ */

