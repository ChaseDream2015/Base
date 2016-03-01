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
* ECUDPSocket.cpp
* This file for ECUDPSocket interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECUDPSocket.h"

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


ECUDPSocket::ECUDPSocket()
:ECSocket(-1)
{
    m_socket = socket(AF_INET, SOCK_DGRAM, 0);
}

ECUDPSocket::ECUDPSocket(EC_INT socket)
:ECSocket(socket)
{
}

ECUDPSocket::~ECUDPSocket()
{
}

EC_INT ECUDPSocket::SendTo(void* pData, int nDataLen, ECSocketAddress *pAddress)
{
    EC_INT nRet = EC_Err_None;
    if (pAddress)
    {
        sockaddr_in destAddr;
        destAddr.sin_family = AF_INET;
        destAddr.sin_port = htons(pAddress->nPort);
        destAddr.sin_addr.s_addr = inet_addr(pAddress->strIP.ToCStr());
        nRet = sendto(m_socket, (const char*)pData, nDataLen, 0, (sockaddr*)(&destAddr), sizeof(destAddr));
    }
    else
    {
        nRet = EC_Err_BadParam;
    }
    return nRet;
}

EC_INT ECUDPSocket::RecvFrom(void* pBuffer, int nMaxBufSize, ECSocketAddress *pAddress)
{
    EC_INT nRet = 0;
    int fromAddrLen;
    sockaddr_in fromAddr;
    nRet = recvfrom(m_socket, (char*)pBuffer, nMaxBufSize, 0, (sockaddr*)(&fromAddr), &fromAddrLen);
    if (pAddress)
    {
        char* pAddrStr = EC_NULL;
        pAddrStr = inet_ntoa(((sockaddr_in*)(&fromAddr))->sin_addr);
        pAddress->nPort = fromAddr.sin_port;
        pAddress->strIP = ECString(pAddrStr);
    }
    else
    {
        nRet = EC_Err_BadParam;
    }
    return nRet;
}
