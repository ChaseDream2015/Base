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
* ECTCPSocket.cpp
* This file for ECTCPSocket interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECTCPSocket.h"

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


ECTCPSocket::ECTCPSocket()
:ECSocket(-1)
{
    m_socket = socket(AF_INET, SOCK_STREAM, 0);
}

ECTCPSocket::ECTCPSocket(EC_INT socket)
:ECSocket(socket)
{
}

ECTCPSocket::~ECTCPSocket()
{
}

EC_INT ECTCPSocket::Send(void* pData, int nDataLen)
{
    return send(m_socket, (const char*)pData, nDataLen, 0);
}

EC_INT ECTCPSocket::Recv(void* pBuffer, int nMaxBufSize)
{
    return recv(m_socket, (char*)pBuffer, nMaxBufSize, 0);
}

EC_INT ECTCPSocket::Connect(ECSocketAddress *pAddress)
{
    EC_INT nRet = 0;
    if (pAddress)
    {
#ifdef EC_OS_Win32
        SOCKADDR_IN addrSrv;
        addrSrv.sin_family = AF_INET;
        addrSrv.sin_port = htons(pAddress->nPort);
        addrSrv.sin_addr.S_un.S_addr = inet_addr(pAddress->strIP.ToCStr());
        nRet = connect(m_socket, (SOCKADDR*)&addrSrv, sizeof(SOCKADDR));
#elif defined EC_OS_Linux
        int address_len;
        struct sockaddr_in address;
        address_len = sizeof(address);
        address.sin_family = AF_INET;
        address.sin_port = htons(pAddress->nPort);
        address.sin_addr.s_addr = inet_addr(pAddress->strIP.ToCStr());
        nRet = connect(m_socket, (struct sockaddr *)&address, address_len);
#elif defined EC_OS_MacOS
        /* TODO */
#elif defined EC_OS_iOS
        /* TODO */
#elif defined EC_OS_Android
        /* TODO */
#endif
    }
    else
    {
        nRet = EC_Err_BadParam;
    }
    return nRet;
}

EC_INT ECTCPSocket::Listen(EC_U32 nMaxConnectionNumber)
{
    return listen(m_socket, nMaxConnectionNumber);
}

ECTCPSocket ECTCPSocket::Accept(ECSocketAddress *pAddress)
{
    EC_INT socket = -1;

#ifdef EC_OS_Win32
    SOCKADDR_IN address;
    int nAddrLen = sizeof(SOCKADDR);
    socket = accept(m_socket, (SOCKADDR*)&address, &nAddrLen);
#elif defined EC_OS_Linux
    struct sockaddr_in address;
    int nAddrLen = sizeof (struct sockaddr_in);
    socket = accept(m_socket, &address, &nAddrLen);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    /* TODO */
#elif defined EC_OS_Android
    /* TODO */
#endif
    if (pAddress)
    {
        char* pAddrStr = EC_NULL;
        pAddrStr = inet_ntoa( ((sockaddr_in*)(&address))->sin_addr ) ;
        pAddress->nPort = address.sin_port;
        pAddress->strIP = ECString(pAddrStr);
    }

    return ECTCPSocket(socket);
}

EC_INT ECTCPSocket::Shutdown(EC_U32 nType)
{
    return shutdown(m_socket, nType);
}
