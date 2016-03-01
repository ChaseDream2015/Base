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
* ECSocket.cpp
* This file for ECSocket interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECSocket.h"

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

ECSocket::ECSocket(EC_INT socket)
{
    m_socket = socket;
}

ECSocket::~ECSocket()
{
}

EC_VOID ECSocket::Close()
{
#ifdef EC_OS_Win32
    closesocket(m_socket);;
#elif defined EC_OS_Linux
    close(m_socket);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    /* TODO */
#elif defined EC_OS_Android
    /* TODO */
#endif
}

EC_INT ECSocket::BindAddress(ECSocketAddress *pAddress)
{
    if (EC_NULL == pAddress)
        return EC_Err_BadParam;
    
    EC_INT nRet = 0;
#ifdef EC_OS_Win32
    SOCKADDR_IN addrSrv;
    addrSrv.sin_family = AF_INET;
    addrSrv.sin_port = htons(pAddress->nPort);
    addrSrv.sin_addr.S_un.S_addr = inet_addr(pAddress->strIP.ToCStr()); 
    nRet = bind(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
#elif defined EC_OS_Linux
    struct sockaddr_in local_addr;
    local_addr.sin_family = AF_INET;
    local_addr.sin_port = htons(pAddress->nPort);
    local_addr.sin_addr.s_addr = inet_addr(pAddress->strIP.ToCStr());   
    int addr_len = sizeof(struct sockaddr_in);
    nRet = bind(socket_fd, (const struct sockaddr*)&local_addr, addr_len)
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    /* TODO */
#elif defined EC_OS_Android
    /* TODO */
#endif
    return nRet;
}

EC_INT ECSocket::SetOptions(int nLevel, int nOptName, const char* pOptVal, int nOptLen)
{
    EC_INT nRet = 0;
#ifdef EC_OS_Win32
    nRet = setsockopt(m_socket, nLevel, nOptName, pOptVal, nOptLen);
#elif defined EC_OS_Linux
    nRet = setsockopt(m_socket, nLevel, nOptName, pOptVal, nOptLen);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    /* TODO */
#elif defined EC_OS_Android
    /* TODO */
#endif
    return nRet;
}
