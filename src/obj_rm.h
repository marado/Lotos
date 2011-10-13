/* vi: set ts=4 sw=4 ai: */
/*
 * obj_rm.h
 *
 *   Lotos v1.2.1  : (c) 1999-2001 Pavol Hluchy (Lopo)
 *   last update   : 26.12.2001
 *   email         : lopo@losys.sk
 *   homepage      : lopo.losys.sk
 *   Lotos homepage: lotos.losys.sk
 */

#ifndef __OBJ_RM_H__
#define __OBJ_RM_H__ 1

/* room information structure */
struct room_struct {
	char name[ROOM_NAME_LEN+1],label[ROOM_LABEL_LEN+1],desc[ROOM_DESC_LEN+1];
	char topic[TOPIC_LEN+1],revbuff[REVIEW_LINES][REVIEW_LEN+2],map[ROOM_NAME_LEN+1];
	int access; /* public , private etc */
	int revline; /* line number for review */
	int mesg_cnt;
	char link_label[MAX_LINKS][ROOM_LABEL_LEN+1]; /* temp store for parse */
	struct room_struct *link[MAX_LINKS];
	struct room_struct *next,*prev;
#ifdef NETLINKS
	int inlink; /* 1 if room accepts incoming net links */
	char netlink_name[SERV_NAME_LEN+1]; /* temp store for config parse */
	struct netlink_struct *netlink; /* for net links, 1 per room */
#endif
	/* rozsirenie pre transporty */
	struct transport_struct *transp;
	};
typedef struct room_struct *RM_OBJECT;

#endif /* obj_rm.h */

