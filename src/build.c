/* vi: set ts=4 sw=4 ai: */
/*
 * build.c
 *
 *   Lotos v1.2.3  : (c) 1999-2003 Pavol Hluchy (Lopo)
 *   last update   : 30.1.2003
 *   email         : lotos@losys.sk
 *   homepage      : lotos.losys.sk
 */

#ifndef __BUILD_C__
#define __BUILD_C__ 1

#include <stdio.h>
#include <string.h>

#include "define.h"
#include "prototypes.h"

void build_datetime(char *str)
{
	set_crash();
	sprintf(str, "%s   %s", __DATE__, __TIME__);
#ifdef DEBUG
	strcat(str, "  ~FR(DEBUG version)");
#endif
}

#endif /* __BUILD_C__ */

