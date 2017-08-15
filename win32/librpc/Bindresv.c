/**********************************************************************
 * ONC RPC for WIN32.
 * 1997 by WD Klotz
 * ESRF, BP 220, F-38640 Grenoble, CEDEX
 * klotz-tech@esrf.fr
 *
 * SUN's ONC RPC for Windows NT and Windows 95. Ammended port from
 * Martin F.Gergeleit's distribution. This version has been modified
 * and cleaned, such as to be compatible with Windows NT and Windows 95.
 * Compiler: MSVC++ version 4.2 and 5.0.
 *
 * Users may use, copy or modify Sun RPC for the Windows NT Operating
 * System according to the Sun copyright below.
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 **********************************************************************/
/*********************************************************************
 * RPC for the Windows NT Operating System
 * 1993 by Martin F. Gergeleit
 * Users may use, copy or modify Sun RPC for the Windows NT Operating
 * System according to the Sun copyright below.
 *
 * RPC for the Windows NT Operating System COMES WITH ABSOLUTELY NO
 * WARRANTY, NOR WILL I BE LIABLE FOR ANY DAMAGES INCURRED FROM THE
 * USE OF. USE ENTIRELY AT YOUR OWN RISK!!!
 *********************************************************************/
#include "all_oncrpc.h"

#ifdef __cplusplus
extern "C" {
#endif

static  char sccsid[] = "@(#)bindresvport.c	2.2 88/07/29 4.0 RPCSRC 1.8 88/02/08 SMI";
/*
 * Sun RPC is a product of Sun Microsystems, Inc. and is provided for
 * unrestricted use provided that this legend is included on all tape
 * media and as a part of the software program in whole or part.  Users
 * may copy or modify Sun RPC without charge, but are not authorized
 * to license or distribute it to anyone else except as part of a product or
 * program developed by the user.
 *
 * SUN RPC IS PROVIDED AS IS WITH NO WARRANTIES OF ANY KIND INCLUDING THE
 * WARRANTIES OF DESIGN, MERCHANTIBILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE, OR ARISING FROM A COURSE OF DEALING, USAGE OR TRADE PRACTICE.
 *
 * Sun RPC is provided with no support and without any obligation on the
 * part of Sun Microsystems, Inc. to assist in its use, correction,
 * modification or enhancement.
 *
 * SUN MICROSYSTEMS, INC. SHALL HAVE NO LIABILITY WITH RESPECT TO THE
 * INFRINGEMENT OF COPYRIGHTS, TRADE SECRETS OR ANY PATENTS BY SUN RPC
 * OR ANY PART THEREOF.
 *
 * In no event will Sun Microsystems, Inc. be liable for any lost revenue
 * or profits or other special, indirect and consequential damages, even if
 * Sun has been advised of the possibility of such damages.
 *
 * Sun Microsystems, Inc.
 * 2550 Garcia Avenue
 * Mountain View, California  94043
 */

/*
 * Copyright (c) 1987 by Sun Microsystems, Inc.
 */



/*
 * Bind a socket to a privileged IP port
 */
int bindresvport(int sd, struct sockaddr_in* sin) {
    int res;
    static short port;
    struct sockaddr_in myaddr;
#ifdef WIN32
    int my_errno;
#else
    extern int errno;
#endif
    int i;

#define STARTPORT 600
#define ENDPORT (IPPORT_RESERVED - 1)
#define NPORTS	(ENDPORT - STARTPORT + 1)

    if (sin == (struct sockaddr_in*)0) {
        sin = &myaddr;
        //		bzero(sin, sizeof (*sin));
        bzero((char*)sin, sizeof(*sin));
        sin->sin_family = AF_INET;
    } else if (sin->sin_family != AF_INET) {
#ifdef WIN32
        my_errno = WSAEPFNOSUPPORT;
#else
        errno = EPFNOSUPPORT;
#endif

        return (-1);
    }
    if (port == 0) {
        port = (getpid() % NPORTS) + STARTPORT;
    }
    res = -1;
#ifdef WIN32
    my_errno = WSAEADDRINUSE;
    for (i = 0; i < NPORTS && res < 0 && my_errno == WSAEADDRINUSE; i++) {
#else
    errno = EADDRINUSE;
    for (i = 0; i < NPORTS && res < 0 && errno == EADDRINUSE; i++) {
#endif
        sin->sin_port = htons(port++);
        if (port > ENDPORT) {
            port = STARTPORT;
        }
        //		res = bind(sd, sin, sizeof(struct sockaddr_in));
        res = bind(sd, (struct sockaddr*)sin, sizeof(struct sockaddr_in));
#ifdef WIN32
        my_errno = WSAerrno;
#endif
    }
    return (res);
}

#ifdef __cplusplus
}
#endif