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
* ECMsgWorker.cpp
* This file for ECMsgWorker interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECOSUtil.h"
#include "ECStringOP.h"
#include "ECMsgWorker.h"


ECMsgWorker::ECMsgWorker(
                         ECMsgHandlerI* pMsgHandler, 
                         EC_PCHAR pThreadName
                         )
:m_strName(pThreadName)
,m_nStatus(ECMsgWorkerStatus_Init)
,m_pMsgHandler(pMsgHandler)
{
#ifdef EC_OS_Win32
    m_ThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)MsgProcEntry, this, 0, NULL);
#elif defined EC_OS_Linux
    pthread_create(&m_ThreadHandle, NULL, MsgProcEntry, this);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_create(&m_ThreadHandle, NULL, MsgProcEntry, this);
#elif defined EC_OS_Android
    /* TODO */
#endif
}

ECMsgWorker::~ECMsgWorker()
{
    Stop();
    ClearMsg();
}

void ECMsgWorker::Run()
{
    if( (ECMsgWorkerStatus_Init == m_nStatus) ||
        (ECMsgWorkerStatus_Pause == m_nStatus) )
    {
        m_nStatus = ECMsgWorkerStatus_Run;
        m_sigPause.Send();
    }
}

void ECMsgWorker::Pause()
{
    if(ECMsgWorkerStatus_Run == m_nStatus)
    {
        m_nStatus = ECMsgWorkerStatus_Pause;
        m_semMsg.SemPost();
    }
}

void ECMsgWorker::Stop()
{
    if (ECMsgWorkerStatus_Stop != m_nStatus)
    {
        EC_U32 nOldStatus = m_nStatus;
        m_nStatus = ECMsgWorkerStatus_Stop;
        if (ECMsgWorkerStatus_Run == nOldStatus)
            m_semMsg.SemPost();
        else if (ECMsgWorkerStatus_Init == nOldStatus)
            m_sigPause.Send();
        else if (ECMsgWorkerStatus_Pause == nOldStatus)
            m_sigPause.Send();
        WaitMsgProcExit();
    }
}

void ECMsgWorker::ClearMsg()
{
    if(m_pMsgHandler) m_pMsgHandler->CleanMsg();
}

EC_U32 ECMsgWorker::PushMsg(void* pMsg)
{
    if (ECMsgWorkerStatus_Run != m_nStatus)
        return EC_Err_OperatorFaild;

    EC_U32 nRet = EC_Err_None;
    if(m_pMsgHandler)
    {
        nRet = m_pMsgHandler->PushMsg(pMsg);
        if(EC_Err_None == nRet)
        {
            m_semMsg.SemPost();
        }
    }
    return nRet;
}

void ECMsgWorker::WaitMsgProcExit()
{
#ifdef EC_OS_Win32
    //WaitForSingleObject(m_TaskThreadHandle, INFINITE);
    WaitForSingleObject(m_ThreadHandle, MAX_EXIT_WAIT);
    CloseHandle(m_ThreadHandle);
#elif defined EC_OS_Linux
    pthread_join(m_ThreadHandle, NULL);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_join(m_ThreadHandle, NULL);
#elif defined EC_OS_Android
    /* TODO */
#endif
}

void* ECMsgWorker::MsgProcEntry(void* pArg)
{
    if(EC_NULL == pArg)
        return EC_NULL;

    ECMsgWorker* pMsgWorker = (ECMsgWorker*)pArg;
    while(EC_TRUE)
    {
        switch (pMsgWorker->m_nStatus)
        {
            case ECMsgWorkerStatus_Init:
            case ECMsgWorkerStatus_Pause:
            {
                pMsgWorker->m_sigPause.Wait();
            }
            break;

            case ECMsgWorkerStatus_Run:
            {
                if (pMsgWorker->m_pMsgHandler)
                {
                    void *pMsg = EC_NULL;
                    EC_U32 nRet = pMsgWorker->m_pMsgHandler->PopMsg(&pMsg);
                    if (EC_Err_None == nRet)
                    {
                        pMsgWorker->m_pMsgHandler->DispatchMsg(pMsg);
                        pMsgWorker->m_pMsgHandler->DestroyMsg(pMsg);
                    }
                }
                pMsgWorker->m_semMsg.SemWait();
            }
            break;

            case ECMsgWorkerStatus_Stop:
            {
                return EC_NULL;
            }
            break;

            default: return EC_NULL;
        }
    }
}
