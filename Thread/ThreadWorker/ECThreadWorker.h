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
* ECThreadWorker.h
* This file for ECThreadWorker define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_THREADWORKER_H
#define EC_THREADWORKER_H

#include "ECMutex.h"
#include "ECArray.h"
#include "ECString.h"
#include "ECThread.h"
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

#define LOOP_CHECK_TIME  1000
#define MAX_THREAD_COUNT 1024

typedef struct
{
    ECString strWorkName;
    EC_U32 nMaxThreadCount;
    EC_U32 nActiveThreadCount;
    const ECThreadInfo** pAllThreadInfo;
}ECThreadWorkerInfo;

class ECThreadWorker
{
public:
    ECThreadWorker(
        EC_U32 nCapability = MAX_THREAD_COUNT,
        EC_PCHAR pThreadWorkerName = EC_NULL
                  );
    ~ECThreadWorker();
    EC_VOID WaitStopAllThreads();
    EC_VOID ForceStopAllThreads();
    const ECThreadWorkerInfo* GetThreadWorkerInfo();
    EC_U32 AddThread(void*(*ThreadProcEntry)(void*), void* pParam, EC_PCHAR pThreadName = EC_NULL);

private:
    EC_VOID UpdateThreadArray();
    EC_S32 FindUseableThreadIndex();
    static void* ThreadManagerProcgEntry(void* pArg);

private:
    BOOL                   m_Exit;
    ECMutex                m_mtxArray;
    ECSemaphore            m_semWorker;
    EC_U32                 m_nCapabilty;
    BOOL                   m_WaitAllStop;
    ECSignal               m_sigAllStoped;
    ECString               m_strWokerName;
    ECThread*              m_pThreadArray;
    ECThread*              m_pThreadManager;
    ECThreadWorkerInfo     m_ThreadWorkInfo;
    EC_U32                 m_nActiveThreadCount;
};

#endif /* EC_THREADWORKER_H */
