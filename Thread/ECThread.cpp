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
* ECThread.cpp
* This file for ECThread interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECThread.h"
#include "ECOSUtil.h"
#include "ECStringOP.h"
#include "ECAutoLock.h"

#define MAX_EXIT_WAIT 500

ECThread::ECThread()
:m_strThreadName(EC_NULL)
{
    m_pThreadArg = EC_NULL;
    m_ThreadProcEntry = EC_NULL;
    m_nStatus = ECThreadStatus_Error;
}

ECThread::ECThread(void*(*ThreadProcEntry)(void*),
                   void* pThreadArg, EC_PCHAR pThreadName)
:m_pThreadArg(pThreadArg)
,m_strThreadName(pThreadName)
,m_ThreadProcEntry(ThreadProcEntry)
,m_nStatus(ECThreadStatus_Init)
{
}

ECThread::~ECThread()
{
    ForceStop();
}

EC_VOID ECThread::Start()
{
    if (ECThreadStatus_Init != m_nStatus) return;
#ifdef EC_OS_Win32
    m_hTheadHandle = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)m_ThreadProcEntry, m_pThreadArg, 0, NULL);
#elif defined EC_OS_Linux
    pthread_create(&m_hTheadHandle, NULL, m_ThreadProcEntry, m_pThreadArg);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_create(&m_hTheadHandle, NULL, m_ThreadProcEntry, m_pThreadArg);
#elif defined EC_OS_Android
    /* TODO */
#endif
    m_nStatus = ECThreadStatus_Running;
}

EC_VOID ECThread::WaitStop()
{
    if (ECThreadStatus_Running != m_nStatus) return;
#ifdef EC_OS_Win32
    WaitForSingleObject(m_hTheadHandle, INFINITE);
    CloseHandle(m_hTheadHandle);
#elif defined EC_OS_Linux
    pthread_join(m_hTheadHandle, NULL);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_join(m_hTheadHandle, NULL);
#elif defined EC_OS_Android
    /* TODO */
#endif
    m_nStatus = ECThreadStatus_Stoped;
}

EC_VOID ECThread::ForceStop()
{
    if (ECThreadStatus_Running != m_nStatus) return;
#ifdef EC_OS_Win32
    TerminateThread(m_hTheadHandle, -1);
    CloseHandle(m_hTheadHandle);
#elif defined EC_OS_Linux
    pthread_cancel(m_hTheadHandle);
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    pthread_cancel(m_hTheadHandle);
#elif defined EC_OS_Android
    /* TODO */
#endif
    m_nStatus = ECThreadStatus_Stoped;
}

EC_BOOL ECThread::IsThreadActive()
{
    UpdateStatus();
    return (ECThreadStatus_Running == m_nStatus);
}

const ECThreadInfo* ECThread::GetThreadInfo()
{
    UpdateStatus();
    m_ThreadInfo.nStatus = m_nStatus;
    m_ThreadInfo.hTheadHandle = m_hTheadHandle;
    m_ThreadInfo.strThreadName = m_strThreadName;

    return &m_ThreadInfo;
}

EC_VOID ECThread::UpdateStatus()
{
    EC_BOOL isStoped = EC_FALSE;
    if(ECThreadStatus_Running == m_nStatus)
    {
#ifdef EC_OS_Win32
        DWORD exitCode = -1;
        GetExitCodeThread(m_hTheadHandle, &exitCode);
        isStoped = (exitCode != STILL_ACTIVE);
#elif defined EC_OS_Linux
        int ret = pthread_kill(m_hTheadHandle, 0);
        isStoped = ((ret == ESRCH) || (ret == EINVAL));
#elif defined EC_OS_MacOS
        /* TODO */
#elif defined EC_OS_iOS
        int ret = pthread_kill(m_hTheadHandle, 0);
        isStoped = ((ret == ESRCH) || (ret == EINVAL))
#elif defined EC_OS_Android
        /* TODO */
#endif
        if (isStoped)
            m_nStatus = ECThreadStatus_Stoped;
    }
}