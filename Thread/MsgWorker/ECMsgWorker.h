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
* ECMsgWorker.h
* This file for ECMsgWorker define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_MSGWORKER_H
#define EC_MSGWORKER_H

#include "ECString.h"
#include "ECSignal.h"
#include "ECSemaphore.h"

#ifdef EC_OS_Win32
#include <windows.h>
#elif defined EC_OS_Linux
#include <pthread.h>
#elif defined EC_OS_MacOS
/* TODO */
#elif defined EC_OS_iOS
#include <pthread.h>
#elif defined EC_OS_Android
/* TODO */
#endif

typedef enum
{
    ECMsgWorkerStatus_Init,
    ECMsgWorkerStatus_Run,
    ECMsgWorkerStatus_Pause,
    ECMsgWorkerStatus_Stop,
    ECMsgWorkerStatus_Unknow = EC_U16_MAX,
} ECMsgWorkerStatus;

#define MAX_EXIT_WAIT   500

class ECMsgHandlerI
{
public:
    virtual ~ECMsgHandlerI(){};
    virtual EC_VOID CleanMsg() = 0;
    virtual EC_U32 PushMsg(void* pMsg) = 0;
    virtual EC_U32 PopMsg(void** ppMsg) = 0;
    virtual EC_VOID DestroyMsg(void* pMsg) = 0;
    virtual EC_VOID  DispatchMsg(void* pMsg = EC_NULL) = 0;
}; /* Need added lock for mutil thread access msg queue */

class ECMsgWorker
{
public:
    ECMsgWorker(
                ECMsgHandlerI* pMsgHandler,
                EC_PCHAR pThreadName = EC_NULL
               );
    ~ECMsgWorker();

    void Run();
    void Pause();
    void Stop();
    void ClearMsg();
    EC_U32 PushMsg(void* pMsg = EC_NULL);

private:
    void WaitMsgProcExit();
    static void* MsgProcEntry(void* pArg);

private:
    ECString            m_strName;
    ECSignal            m_sigPause;
    ECSemaphore         m_semMsg;
    ECMsgHandlerI*      m_pMsgHandler;
    ecThreadHandle      m_ThreadHandle;
    ECMsgWorkerStatus   m_nStatus;
};

#endif /* EC_MSGWORKER_H */
