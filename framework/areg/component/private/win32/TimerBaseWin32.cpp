/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2022 Aregtech UG. All rights reserved.
 * \file        areg/component/private/win32/TimerBaseWin32.cpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit
 * \author      Artak Avetyan
 * \brief       AREG Platform, Timer base class implementation.
 *              Windows OS specific implementation.
 *
 ************************************************************************/

#include "areg/component/TimerBase.hpp"

#ifdef  _WINDOWS

#include <windows.h>

bool TimerBase::createWaitableTimer()
{
    Lock lock(mLock);

    if ((mHandle == nullptr) && (mTimeoutInMs != NECommon::INVALID_TIMEOUT))
    {
        TCHAR* name = nullptr;
        TCHAR convertName[MAX_PATH];

        if (mName.isEmpty() == false)
        {
            NEString::copyString<TCHAR, char>(convertName, MAX_PATH, mName.getString(), mName.getLength());
            name = convertName;
        }
        
        mHandle = static_cast<TIMERHANDLE>(::CreateWaitableTimer(nullptr, FALSE, name));

#ifdef _DEBUG
        if (mHandle == nullptr)
        {
            OUTPUT_ERR("Failed creating timer [ %s ], the system error code is [ 0x%p ]", mName.getString(), GetLastError());
        }
#endif // _DEBUG
    }

    return (mHandle != nullptr);
}

void TimerBase::destroyWaitableTimer( void )
{
    Lock lock(mLock);

    TIMERHANDLE handle  = mHandle;
    mHandle = nullptr;
    if (handle != nullptr)
    {
        ::CancelWaitableTimer(static_cast<HANDLE>(handle));
        ::CloseHandle(static_cast<HANDLE>(handle));
    }
}

#endif // _WINDOWS