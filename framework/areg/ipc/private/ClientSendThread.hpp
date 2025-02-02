#ifndef AREG_IPC_PRIVATE_CLIENTSENDTHREAD_HPP
#define AREG_IPC_PRIVATE_CLIENTSENDTHREAD_HPP
/************************************************************************
 * This file is part of the AREG SDK core engine.
 * AREG SDK is dual-licensed under Free open source (Apache version 2.0
 * License) and Commercial (with various pricing models) licenses, depending
 * on the nature of the project (commercial, research, academic or free).
 * You should have received a copy of the AREG SDK license description in LICENSE.txt.
 * If not, please contact to info[at]aregtech.com
 *
 * \copyright   (c) 2017-2022 Aregtech UG. All rights reserved.
 * \file        areg/ipc/private/ClientSendThread.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework
 * \author      Artak Avetyan
 * \brief       AREG Platform Send Message Thread
 ************************************************************************/

/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/component/DispatcherThread.hpp"
#include "areg/ipc/RemoteServiceEvent.hpp"

#include <atomic>

 /************************************************************************
 * Dependencies
 ************************************************************************/
class IERemoteServiceHandler;
class ClientConnection;

//////////////////////////////////////////////////////////////////////////
// ClientSendThread class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   The message sender thread. All messages to be sent to remote routing service
 *          are queued in message sender thread. 
 **/
class ClientSendThread  : public    DispatcherThread
                        , public    IESendMessageEventConsumer
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Initializes Service handler and client connection objects.
     * \param   remoteService   The instance of remote service to process messages.
     * \param   connection      The instance of client connection object to send messages.
     **/
    ClientSendThread( IERemoteServiceHandler & remoteService, ClientConnection & connection );
    /**
     * \brief   Destructor
     **/
    virtual ~ClientSendThread( void ) = default;

/************************************************************************/
// Actions and attributes.
/************************************************************************/
public:
    /**
     * \brief   Returns accumulative value of sent data size and rests the existing value to zero.
     *          The operations are atomic. The value can be used to display data rate, for example.
     **/
    inline uint32_t extractDataSend( void );

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
    virtual bool runDispatcher( void ) override;

/************************************************************************/
// IEEventRouter interface overrides
/************************************************************************/

    /**
     * \brief	Posts event and delivers to its target.
     *          Since the Dispatcher Thread is a Base object for
     *          Worker and Component threads, it does nothing
     *          and only destroys event object without processing.
     *          Override this method or use Worker / Component thread.
     * \param	eventElem	Event object to post
     * \return	In this class it always returns true.
     **/
    virtual bool postEvent( Event & eventElem ) override;

private:
/************************************************************************/
// IESendMessageEventConsumer interface overrides.
/************************************************************************/
    /**
     * \brief   Automatically triggered when event is dispatched by registered
     *          worker / component thread.
     * \param   data    The data object passed in event. It should have at least
     *                  default constructor and assigning operator.
     *                  This object is not used for IPC.
     **/
    virtual void processEvent( const SendMessageEventData & data ) override;

//////////////////////////////////////////////////////////////////////////
// Member variables.
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   The instance of remote service handler to dispatch messages.
     **/
    IERemoteServiceHandler &    mRemoteService;
    /**
     * \brief   The instance of connection to send messages from remote routing service.
     **/
    ClientConnection &          mConnection;

    /**
     * \brief   Accumulative value of sent data size.
     **/
    std::atomic_uint             mBytesSend;

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    ClientSendThread( void ) = delete;
    DECLARE_NOCOPY_NOMOVE( ClientSendThread );
};

inline uint32_t ClientSendThread::extractDataSend( void )
{
    return static_cast<uint32_t>(mBytesSend.exchange( 0 ));
}

#endif  // AREG_IPC_PRIVATE_CLIENTSENDTHREAD_HPP
