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
* ECSemaphore.h
* This file for ECSemaphore interface implementation.
*
* Eamil:   epengao@126.com
* Author:  Peter Gao
* Version: Intial first version.
* --------------------------------------------------------------------
*/

#include "ECOSUtil.h"
#include "ECStringOP.h"
#include "ECSemaphore.h"


ECSemaphore::ECSemaphore(EC_U32 nInitSig, /*0*/
                         EC_U32 nMaxSig   /*EC_MAX_U16*/)
{
#ifdef EC_OS_Win32
    m_hSemaphore = CreateSemaphore
                    (
                        EC_NULL,  // default security attributes
                        nInitSig,  // initial count
                        nMaxSig,   // maximum count
                        EC_NULL   // unnamed semaphore
                    );
#elif defined EC_OS_Linux
    /* TODO */
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    //m_pSemaphoreName = new EC_CHAR[128];
    //ECStringOP::IntToStr(m_pSemaphoreName, (EC_U32)(ecGetSystemTimeU() % EC_U32_MAX));
    //m_hSemaphore = sem_open(m_pSemaphoreName, O_CREAT, 644, nInitSig);
    signal_times = 0;
    pthread_cond_init(&cond, EC_NULL);
#elif defined EC_OS_Android
    /* TODO */
#endif
}

ECSemaphore::~ECSemaphore()
{
#ifdef EC_OS_Win32
    CloseHandle(m_hSemaphore);
#elif defined EC_OS_Linux
    /* TODO */
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    //sem_close(m_hSemaphore);
    //sem_unlink(m_pSemaphoreName);
    //if(m_pSemaphoreName) delete m_pSemaphoreName;
    pthread_cond_destroy(&cond);
#elif defined EC_OS_Android
    /* TODO */
#endif
}

EC_BOOL ECSemaphore::SemPost()
{
#ifdef EC_OS_Win32
    return ReleaseSemaphore(m_hSemaphore, 1, EC_NULL);
#else
    //return (0 == sem_post(m_hSemaphore));
    signal_times++;
    return (0 == pthread_cond_signal(&cond));
#endif
}

EC_BOOL ECSemaphore::SemWait()
{
#ifdef EC_OS_Win32
    return (0 == WaitForSingleObject(m_hSemaphore, INFINITE));
#elif defined EC_OS_Linux
    /* TODO */
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    //return (0 == sem_wait(m_hSemaphore));
    signal_times--;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, EC_NULL);
    while (signal_times < 0)
    {
        pthread_cond_wait(&cond, &mutex);
    }
    pthread_mutex_destroy(&mutex);
    return EC_TRUE;
#elif defined EC_OS_Android
    /* TODO */
#endif
}

EC_BOOL ECSemaphore::TimedSemWait(EC_U32 nTimeout)
{
#ifdef EC_OS_Win32
    return (0 == WaitForSingleObject(m_hSemaphore, nTimeout));
#elif defined EC_OS_Linux
    /* TODO */
#elif defined EC_OS_MacOS
    /* TODO */
#elif defined EC_OS_iOS
    //struct timespec timeout;
    //timeout.tv_nsec = 0;
    //timeout.tv_sec = time(NULL) + nTimeout;
    //return (0 == sem_timedwait(m_hSemaphore, nTimeout));
    struct timespec timeout;
    timeout.tv_sec = time(NULL) + secs;
    timeout.tv_nsec = 0;

    signal_times--;
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, EC_NULL);
    while (signal_times < 0)
    {
        pthread_cond_timedwait(&cond, &mutex, &timeout);
    }
    pthread_mutex_destroy(&mutex);
    return EC_TRUE;
#elif defined EC_OS_Android
    /* TODO */
#endif
}
