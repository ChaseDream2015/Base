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
* ECTaskWorker.h
* This file for ECTaskWorker define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_TASKWORKER_H
#define EC_TASKWORKER_H

#include "ECString.h"
#include "ECSignal.h"
#include "ECSemaphore.h"
#include "ECRingQueue.h"

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
    ECTaskWorkerStatus_Init,
    ECTaskWorkerStatus_Run,
    ECTaskWorkerStatus_Pause,
    ECTaskWorkerStatus_Stoped,
    ECTaskWorkerStatus_Unknow = EC_U16_MAX,
} ECTaskWorkerStatus;

#define MAX_EXIT_WAIT 500
#define DFT_TASK_SIZE 1024

typedef struct
{
    void* pParam;
    void*(*TaskHandler)(void*);
} ECTask;

class ECTaskWorker
{
public:
    ECTaskWorker(
        EC_U32 nCapability = DFT_TASK_SIZE,
        EC_PCHAR pTaskWorkerName = EC_NULL
                );
    ~ECTaskWorker();

    EC_VOID Run();
    EC_VOID Pause();
    EC_VOID Stop();
    void CancelAllTask();
    EC_U32 AddTask(void*(*TaskHandler)(void*), void* pParam);

private:
    void WaitTaskProcExit();
    static void* TaskThreadEntry(void* pArg);

private:
    ECTaskWorkerStatus      m_nStatus;
    ECMutex            m_mtxTaskQueue;
    ECString           m_strWokerName;
    ECSemaphore        m_semWorkerRun;
    ECSemaphore      m_semWorkerPause;
    ecThreadHandle m_TaskThreadHandle;
    ECRingQueue<ECTask>* m_pTaskQueue;
};

#endif /* EC_TASKWORKER_H */
