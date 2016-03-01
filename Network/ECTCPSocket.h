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
* ECTCPSocket.h
* This file for ECTCPSocket define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_TCP_SOCKET_H
#define EC_TCP_SOCKET_H

#include "ECSocket.h"

class ECTCPSocket : public ECSocket
{
public:
    ECTCPSocket();
    ECTCPSocket(EC_INT socket);
    ~ECTCPSocket();

    EC_INT Send(void* pData, int nDataLen);
    EC_INT Recv(void* pBuffer, int nMaxBufSize);
    EC_INT Connect(ECSocketAddress *pAddress);
    EC_INT Listen(EC_U32 nMaxConnectionNumber = EC_U16_MAX);
    ECTCPSocket Accept(ECSocketAddress *pAddress = EC_NULL);
    EC_INT Shutdown(EC_U32 nType = EC_SOCKET_RD_WR);
};

#endif /* EC_TCP_SOCKET_H */

