/*
* This software is developed for study and improve coding skill ...
*
* Project:  Enjoyable Coding< EC >
* Copyright (C) 2014-2016 Gao Peng

* This library is free software; you can redistribute it and/or
* modify it under the terms of the GNU Library General Public
* License as published by the Free Software Foundation; either
* version 2 of the License, or (at your option) any later version.

* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
* Library General Public License for more details.

* You should have received a copy of the GNU Library General Public
* License along with this library; if not, write to the Free
* Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA

* ---------------------------------------------------------------------
* ECSocket.h
* This file for ECSocket define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_SOCKET_H
#define EC_SOCKET_H

#include "ECString.h"

#ifdef EC_OS_Win32
#include <Winsock2.h>
#elif defined EC_OS_Linux
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/socket.h>
#elif defined EC_OS_MacOS
/* TODO */
#elif defined EC_OS_iOS
/* TODO */
#elif defined EC_OS_Android
/* TODO */
#endif

enum
{
    EC_SOCKET_READ  = 0,
    EC_SOCKET_WRITE = 1,
    EC_SOCKET_RD_WR = 2,
};

typedef struct
{
    ECString strIP;
    EC_U16   nPort;
} ECSocketAddress;

class ECSocket
{
public:
    ECSocket(EC_INT socket);
    virtual ~ECSocket();
    virtual EC_VOID Close();
    virtual EC_INT BindAddress(ECSocketAddress *pAddress);
    virtual EC_INT SetOptions(int nLevel, int nOptName, const char* pOptVal, int nOptLen);

protected:
#ifdef EC_OS_Win32
    SOCKET m_socket;
#elif defined EC_OS_Linux
    int m_socket;
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    /* TODO */
#elif defined EC_OS_Android
    /* TODO */
#endif
};

#endif /* EC_SOCKET_H */
