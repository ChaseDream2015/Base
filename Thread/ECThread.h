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
* ECThread.h
* This file for ECThread define and encapsulation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#ifndef EC_THREAD_H
#define EC_THREAD_H

#include "ECString.h"

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
    ECThreadStatus_Init    = 0,
    ECThreadStatus_Running = 1,
    ECThreadStatus_Stoped  = 2,
    ECThreadStatus_Error   = 0xFF,
} ECThreadStatus;

typedef struct
{
    ECString strThreadName;
    ECThreadStatus nStatus;
    ecThreadHandle hTheadHandle;
}ECThreadInfo;

class ECThread
{
public:
    ECThread();
    ECThread(void*(*ThreadProcEntry)(void*),
             void* pThreadArg, EC_PCHAR pTaskWorkerName = EC_NULL);
    ~ECThread();
    EC_VOID Start();
    EC_VOID WaitStop();
    EC_VOID ForceStop();
    EC_BOOL IsThreadActive();
    const ECThreadInfo* GetThreadInfo();
private:
    EC_VOID UpdateStatus();

private:
    void* m_pThreadArg;
    void*(*m_ThreadProcEntry)(void*);
    ECString m_strThreadName;
    ECThreadInfo m_ThreadInfo;
    ECThreadStatus m_nStatus;
    ecThreadHandle m_hTheadHandle;
};

#endif /* EC_THREAD_H */
