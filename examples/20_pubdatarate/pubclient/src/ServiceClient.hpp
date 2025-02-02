#pragma once

/************************************************************************
 * \file        locservice/src/ServiceClient.hpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework examples
 * \author      Artak Avetyan
 * \brief       Collection of AREG SDK examples.
 *              This file contains simple implementation of service client to
 *              request message output
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/

#include "areg/base/GEGlobal.h"
#include "areg/component/Component.hpp"
#include "generated/src/LargeDataClientBase.hpp"
#include "areg/component/IETimerConsumer.hpp"

#include "areg/appbase/Console.hpp"
#include "areg/component/Timer.hpp"
#include "common/NELargeData.hpp"
#include "common/SimpleBitmap.hpp"

//////////////////////////////////////////////////////////////////////////
// ServicingComponent class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief   Service client, connects to the large data service to receive
 *          generated image data. It outputs the statistics of received
 *          data and imte blocks.
 **/
class ServiceClient : public    Component
                    , protected LargeDataClientBase
                    , protected IETimerConsumer
{
private:

    //!< The coordinates to output client application title / headline.
    static constexpr Console::Coord     COORD_TITLE     { 0, 0 };

    //!< Coordinates to output data rate information of large data client.
    static constexpr Console::Coord     COORD_DATA_RATE { 0, 1 };

    //!< File name to save bitmap image.
    static constexpr std::string_view   FILE_NAME       { ".\\SimpleImage.bmp" };

    //!< Timer name.
    static constexpr std::string_view   TIMER_NAME      { "DataRateTimer" };

    //!< Message to output as application title / headline.
    static constexpr std::string_view   MSG_APP_TITLE   { "Application to test data rate, service client part..." };

    //!< Message to output data rate.
    static constexpr std::string_view   MSG_DATA_RATE   { "Receiving data with rate [ % 7.2f ] %s and [ %u ] blocks per second." };

//////////////////////////////////////////////////////////////////////////
// Static methods
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Called by system to instantiate the component.
     * \param   entry   The entry of registry, which describes the component.
     * \param   owner   The component owning thread.
     * \return  Returns instantiated component to run in the system
     **/
    static Component * CreateComponent( const NERegistry::ComponentEntry & entry, ComponentThread & owner );

    /**
     * \brief   Called by system to delete component and free resources.
     * \param   compObject  The instance of component previously created by CreateComponent method.
     * \param   entry   The entry of registry, which describes the component.
     **/
    static void DeleteComponent( Component & compObject, const NERegistry::ComponentEntry & entry );

//////////////////////////////////////////////////////////////////////////
// Constructor / destructor
//////////////////////////////////////////////////////////////////////////
protected:

    /**
     * \brief   Instantiates the component object.
     * \param   entry   The entry of registry, which describes the component.
     * \param   owner   The component owning thread.
     **/
    ServiceClient(const NERegistry::ComponentEntry & entry, ComponentThread & owner);

    /**
     * \brief   Destructor
     **/
    virtual ~ServiceClient(void) = default;

protected:

/************************************************************************/
// Component overrides
/************************************************************************/
    /**
     * \brief	This function is triggered by component thread when it 
     *          requires component to start up. Set listeners and make
     *          initialization in this function call.
     * \param	comThread	The component thread, which triggered startup command
     **/
    virtual void startupComponent( ComponentThread & comThread ) override;

    /**
     * \brief   Server broadcast.
     *          Called to notify all connected clients that new image data is available.
     *          Overwrite, if need to handle Broadcast call of server object.
     *          This call will be automatically triggered, on every appropriate request call
     * \param   imageBlock  Acquired image block.
     **/
    virtual void broadcastImageBlockAcquired( const NELargeData::ImageBlock & imageBlock ) override;

    /**
     * \brief   Server broadcast.
     *          Called to notify all clients that service is shutting down.
     *          Overwrite, if need to handle Broadcast call of server object.
     *          This call will be automatically triggered, on every appropriate request call
     **/
    virtual void broadcastServiceStopping( void ) override;

/************************************************************************/
// IEProxyListener Overrides
/************************************************************************/
    /**
     * \brief   Triggered by Proxy, when gets service connected event.
     *          Make client initializations in this function. No request
     *          will be processed until this function is not called for
     *          client object. Also set listeners here if client is interested
     *          to receive update notifications.
     * \param   isConnected     The flag, indicating whether service is connected
     *                          or disconnected. Make cleanups and stop sending
     *                          requests or assigning for notification if
     *                          this flag is false. No event to Stub target will
     *                          be sent, until no service connected event is
     *                          received.
     * \param   proxy           The Service Interface Proxy object, which is
     *                          notifying service connection.
     * \return  Return true if this service connect notification was relevant to client object,
     *          i.e. if passed Proxy address is equal to the Proxy object that client has.
     *          If Proxy objects are not equal, it should return false;
     **/
    virtual bool serviceConnected( bool isConnected, ProxyBase & proxy ) override;

/************************************************************************/
// IETimerConsumer interface overrides.
/************************************************************************/

    /**
     * \brief   Triggered when Timer is expired. 
     *          The passed Timer parameter is indicating object, which has been expired.
     *          Overwrite method to receive messages.
     * \param   timer   The timer object that is expired.
     **/
    virtual void processTimer( Timer & timer ) override;

//////////////////////////////////////////////////////////////////////////
// member variables
//////////////////////////////////////////////////////////////////////////
private:
    //!< Bitmap object to construct received image data.
    SimpleBitmap    mBitmap;

    //!< Received data size.
    uint32_t        mDataSize;

    //!< Received image blocks
    uint32_t        mBlockCount;

    //!< Timer to output message.
    Timer           mTimer;

//////////////////////////////////////////////////////////////////////////
// hidden methods
//////////////////////////////////////////////////////////////////////////
private:
    inline ServiceClient & self( void );

//////////////////////////////////////////////////////////////////////////
// forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    ServiceClient( void ) = delete;
    DECLARE_NOCOPY_NOMOVE( ServiceClient );
};

inline ServiceClient & ServiceClient::self( void )
{
    return (*this);
}
