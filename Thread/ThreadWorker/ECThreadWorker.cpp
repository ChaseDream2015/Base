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
* ECThreadWorker.cpp
* This file for ECThreadWorker interface & encapsulation implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECError.h"
#include "ECStringOP.h"
#include "ECAutoLock.h"
#include "ECThreadWorker.h"


ECThreadWorker::ECThreadWorker(
    EC_U32 nCapability,
    EC_PCHAR pThreadWorkerName)
:m_Exit(EC_FALSE)
,m_WaitAllStop(EC_FALSE)
,m_nActiveThreadCount(0)
,m_nCapabilty(nCapability)
,m_strWokerName(pThreadWorkerName)
{
    m_ThreadWorkInfo.pAllThreadInfo = EC_NULL;
    m_pThreadArray = new ECThread[nCapability];
    m_pThreadManager = new ECThread(ThreadManagerProcgEntry, this);
    m_pThreadManager->Start();
}

ECThreadWorker::~ECThreadWorker()
{
    if (m_pThreadManager)
    {
        m_Exit = EC_TRUE;
        m_semWorker.SemPost();
        m_pThreadManager->WaitStop();
        delete m_pThreadManager;
    }
    ForceStopAllThreads();
    if (m_pThreadArray)
    {
        delete[]m_pThreadArray;
    }
    if (m_ThreadWorkInfo.pAllThreadInfo)
    {
        delete[]m_ThreadWorkInfo.pAllThreadInfo;
    }
}

EC_U32 ECThreadWorker::AddThread(void*(*ThreadProcEntry)(void*), void* pParam, EC_PCHAR pThreadName)
{
    EC_S32 nIndex = FindUseableThreadIndex();
    if ((nIndex >= 0) || (nIndex < m_nCapabilty))
    {
        ECThread ecThread(ThreadProcEntry, pParam, pThreadName);
        ECAutoLock Lock(&m_mtxArray);
        {
            m_pThreadArray[nIndex] = ecThread;
            m_pThreadArray[nIndex].Start();
            m_nActiveThreadCount++;
        }
        return EC_Err_None;
    }
    return EC_Err_ContainerFull;
}

EC_VOID ECThreadWorker::WaitStopAllThreads()
{
    m_WaitAllStop = EC_TRUE;
    m_sigAllStoped.Wait();
}

EC_VOID ECThreadWorker::ForceStopAllThreads()
{
    ECAutoLock Lock(&m_mtxArray);
    for (EC_U32 i = 0; i < m_nCapabilty; i++)
    {
        if (m_pThreadArray[i].IsThreadActive())
            m_pThreadArray[i].ForceStop();
    }
}

const ECThreadWorkerInfo* ECThreadWorker::GetThreadWorkerInfo()
{
    m_ThreadWorkInfo.strWorkName = m_strWokerName;
    m_ThreadWorkInfo.nMaxThreadCount = m_nCapabilty;
    m_ThreadWorkInfo.nActiveThreadCount = m_nActiveThreadCount;

    if (m_ThreadWorkInfo.pAllThreadInfo == EC_NULL)
    {
        m_ThreadWorkInfo.pAllThreadInfo = new const ECThreadInfo*[m_nCapabilty];
    }
    UpdateThreadArray();
    {
        ECAutoLock Lock(&m_mtxArray);
        for (EC_U32 i = 0; i < m_nCapabilty; i++)
        {
            m_ThreadWorkInfo.pAllThreadInfo[i] = m_pThreadArray[i].GetThreadInfo();
        }
    }
    return &m_ThreadWorkInfo;
}

EC_VOID ECThreadWorker::UpdateThreadArray()
{
    EC_U32 nStopCount = 0;
    ECAutoLock Lock(&m_mtxArray);
    for (EC_U32 i = 0; i < m_nCapabilty; i++)
    {
        if (m_pThreadArray[i].IsThreadActive())
            continue;
        else
            nStopCount++;
    }
    m_nActiveThreadCount = m_nCapabilty - nStopCount;
    if ((m_nActiveThreadCount == 0) && m_WaitAllStop)
    {
        m_sigAllStoped.Send();
    }
}

EC_S32 ECThreadWorker::FindUseableThreadIndex()
{
    UpdateThreadArray();
    ECAutoLock Lock(&m_mtxArray);
    for (EC_U32 i = 0; i < m_nCapabilty; i++)
    {
        if (m_pThreadArray[i].IsThreadActive())
            continue;
        else
            return i;
    }
    return -1;
}

void* ECThreadWorker::ThreadManagerProcgEntry(void* pArg)
{
    ECThreadWorker *pWorker = (ECThreadWorker*)pArg;
    while (EC_TRUE)
    {
        if (pWorker->m_Exit) break;
        pWorker->UpdateThreadArray();
        pWorker->m_semWorker.TimedSemWait(LOOP_CHECK_TIME);
    }
    return EC_NULL;
}
