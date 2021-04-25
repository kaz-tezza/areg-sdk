/************************************************************************
 * \file        areg/src/appbase/private/CEApplicationWin.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Windows OS specific CEApplication methods implementation
 *              Windows apecifix API calls.
 ************************************************************************/
#include "areg/src/appbase/CEApplication.hpp"

#ifdef _WINDOWS

#include <windows.h>

/**
 * \brief   Windows OS specific implementation of method.
 **/
bool CEApplication::_startRouterService( void )
{
    bool result = false;

    DWORD rights = SC_MANAGER_CONNECT | SC_MANAGER_ENUMERATE_SERVICE | SC_MANAGER_QUERY_LOCK_STATUS | STANDARD_RIGHTS_READ;
    SC_HANDLE SeMHandle = ::OpenSCManager( NULL, NULL, rights );
    if ( SeMHandle != NULL )
    {
        rights = SERVICE_PAUSE_CONTINUE | SERVICE_QUERY_STATUS | SERVICE_START | SERVICE_STOP;
        SC_HANDLE SvcHandle = ::OpenService( SeMHandle, NEApplication::ROUTER_SERVICE_NAME_WIDE, rights );
        if ( SvcHandle != NULL )
        {
            SERVICE_STATUS serviceStatus = { 0 };
            if ( ::QueryServiceStatus( SvcHandle, &serviceStatus ) )
            {
                switch ( serviceStatus.dwCurrentState )
                {
                    /*nothing to do*/
                case SERVICE_CONTINUE_PENDING:  // fall through
                case SERVICE_START_PENDING:     // fall through
                case SERVICE_RUNNING:
                    result = true;
                    break;

                    /*service was paused*/
                case SERVICE_PAUSE_PENDING:
                case SERVICE_PAUSED:
                    result = ::ControlService( SvcHandle, SERVICE_CONTROL_CONTINUE, &serviceStatus ) != FALSE;
                    break;

                    /*service not running*/
                case SERVICE_STOP_PENDING:
                case SERVICE_STOPPED:
                    result = ::StartService( SvcHandle, 0, NULL ) != FALSE;
                    break;

                    /*in all other cases*/
                default:
                    break;
                }
            }

            ::CloseServiceHandle( SvcHandle );
        }

        ::CloseServiceHandle( SeMHandle );
    }

    return result;
}
#endif // _WINDOWS
