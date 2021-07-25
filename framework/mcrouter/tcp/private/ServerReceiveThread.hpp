#ifndef MCROUTER_TCP_PRIVATE_SERVERRECEIVETHREAD_HPP
#define MCROUTER_TCP_PRIVATE_SERVERRECEIVETHREAD_HPP

/************************************************************************
 * \file        mcrouter/tcp/private/ServerReceiveThread.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform Receive Message Thread of Server socket.
 ************************************************************************/

/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/component/DispatcherThread.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/
class IEServerConnectionHandler;
class IERemoteServiceHandler;
class ServerConnection;

//////////////////////////////////////////////////////////////////////////
// ServerConnection class declaration.
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The IPC message receiving thread of server socket.
 **/
class ServerReceiveThread    : public    DispatcherThread
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Initializes connection, connection servicing and connection handling objects
     * \param   connectHandler  The instance of server socket connect / disconnect handling interface
     * \param   remoteService   The instance of remote servicing handler
     * \param   connection      The instance of server connection object.
     **/
    ServerReceiveThread( IEServerConnectionHandler & connectHandler, IERemoteServiceHandler & remoteService, ServerConnection & connection );
    /**
     * \brief   Destructor
     **/
    virtual ~ServerReceiveThread( void );

protected:
/************************************************************************/
// DispatcherThread overrides
/************************************************************************/

    /**
     * \brief	Triggered when dispatcher starts running. 
     *          In this function runs main dispatching loop.
     *          Events are picked and dispatched here.
     *          Override if logic should be changed.
     * \return	Returns true if Exit Event is signaled.
     **/
    virtual bool runDispatcher( void );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   The instance of remote servicing interface object
     **/
    IERemoteServiceHandler &   mRemoteService;
    /**
     * \brief   The instance of connection handler interface object
     **/
    IEServerConnectionHandler& mConnectHandler;
    /**
     * \brief   The instance of server connection object
     **/
    ServerConnection &        mConnection;

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    ServerReceiveThread( void );
    ServerReceiveThread( const ServerReceiveThread & /*src*/ );
    const ServerReceiveThread & operator = ( const ServerReceiveThread & /*src*/ );
};

#endif  // MCROUTER_TCP_PRIVATE_SERVERRECEIVETHREAD_HPP