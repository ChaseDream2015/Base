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
* ECTaskWorker.cpp
* This file for ECTaskWorker interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECOSUtil.h"
#include "ECAutoLock.h"
#include "ECStringOP.h"
#include "ECTaskWorker.h"


ECTaskWorker::ECTaskWorker(
    EC_U32 nCapability,
    EC_PCHAR pTaskWorkerName)
:m_pTaskQueue(EC_NULL)
,m_semWorkerPause(0, 1)
,m_strWokerName(pTaskWorkerName)
{
    m_nStatus = ECTaskWorkerStatus_Init;
    m_pTaskQueue = new ECRingQueue<ECTask>(nCapability);
#ifdef EC_OS_Win32
    m_TaskThreadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)TaskThreadEntry, this, 0, NULL);
#elif defined EC_OS_Linux
    pthread_create(&m_TaskThreadHandle, NULL, TaskThreadEntry, this);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_create(&m_TaskThreadHandle, NULL, TaskThreadEntry, this);
#elif defined EC_OS_Android
    /* TODO */
#endif
}

ECTaskWorker::~ECTaskWorker()
{
    Stop();
    CancelAllTask();
    if (m_pTaskQueue) delete m_pTaskQueue;
}

void ECTaskWorker::Run()
{
    if( (ECTaskWorkerStatus_Init == m_nStatus) ||
        (ECTaskWorkerStatus_Pause == m_nStatus) )
    {
        m_nStatus = ECTaskWorkerStatus_Run;
        m_semWorkerPause.SemPost();
    }
}

void ECTaskWorker::Pause()
{
    if (ECTaskWorkerStatus_Run == m_nStatus)
    {
        m_nStatus = ECTaskWorkerStatus_Pause;
        m_semWorkerRun.SemPost();
    }
}

void ECTaskWorker::Stop()
{
    if (ECTaskWorkerStatus_Stoped != m_nStatus)
    {
        EC_U32 nOldStatus = m_nStatus;
        m_nStatus = ECTaskWorkerStatus_Stoped;

        if (ECTaskWorkerStatus_Run == nOldStatus)
            m_semWorkerRun.SemPost();
        if (ECTaskWorkerStatus_Init == nOldStatus)
            m_semWorkerPause.SemPost();
        else if (ECTaskWorkerStatus_Pause == nOldStatus)
            m_semWorkerPause.SemPost();
        WaitTaskProcExit();
    }
}

void ECTaskWorker::CancelAllTask()
{
    ECAutoLock AutoLock(&m_mtxTaskQueue);
    {
        m_pTaskQueue->Clean();
    }
}

EC_U32 ECTaskWorker::AddTask(void*(*TaskHandler)(void*), void* pParam)
{
    EC_U32 nRet;
    ECTask ecTask;
    ecTask.pParam = pParam;
    ecTask.TaskHandler = TaskHandler;
    ECAutoLock AutoLock(&m_mtxTaskQueue);
    {
        nRet = m_pTaskQueue->Push(ecTask);
    }
    m_semWorkerRun.SemPost();
    return nRet;
}

void ECTaskWorker::WaitTaskProcExit()
{
#ifdef EC_OS_Win32
    //WaitForSingleObject(m_TaskThreadHandle, INFINITE);
    WaitForSingleObject(m_TaskThreadHandle, MAX_EXIT_WAIT);
    CloseHandle(m_TaskThreadHandle);
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

void* ECTaskWorker::TaskThreadEntry(void* pArg)
{
    ECTaskWorker* pTaskWorker = (ECTaskWorker*)pArg;
    while(EC_TRUE)
    {
        switch (pTaskWorker->m_nStatus)
        {
            case ECTaskWorkerStatus_Init:
            case ECTaskWorkerStatus_Pause:
            {
                pTaskWorker->m_semWorkerPause.SemWait();
            }
            break;

            case ECTaskWorkerStatus_Run:
            {
                ECTask ecTask;
                EC_U32 nRet = pTaskWorker->m_pTaskQueue->Pop(&ecTask);
                if( (EC_Err_None == nRet) &&
                    (ecTask.TaskHandler != EC_NULL) )
                {
                    ecTask.TaskHandler(ecTask.pParam);
                }
                pTaskWorker->m_semWorkerRun.SemWait();
            }
            break;

            case ECTaskWorkerStatus_Stoped:
            {
                return EC_NULL;
            }
            break;

            default: return EC_NULL;
        }
    }
}
