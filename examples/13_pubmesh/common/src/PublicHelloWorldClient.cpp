/************************************************************************
 * \file        common/src/PublicHelloWorldClient.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework examples
 * \author      Artak Avetyan
 * \brief       Collection of AREG SDK examples.
 *              This file contains simple implementation of service client to
 *              request message output
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "common/src/PublicHelloWorldClient.hpp"

#include "common/src/NECommon.hpp"
#include "areg/component/ComponentThread.hpp"
#include "areg/component/ProxyBase.hpp"
#include "areg/component/Component.hpp"
#include "areg/appbase/Application.hpp"
#include "areg/base/Process.hpp"
#include "areg/trace/GETrace.h"

DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_serviceConnected);
DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_responseRegister);
DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_responseHelloWorld);
DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_broadcastServiceUnavailable);
DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_onServiceStateUpdate);
DEF_TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_processTimer);

PublicHelloWorldClient::PublicHelloWorldClient( const NERegistry::DependencyEntry & dependency, Component & owner, unsigned int timeout)
    : PublicHelloWorldClientBase( dependency, owner )
    , SystemShutdownClientBase  ( NECommon::PublicControllerService, owner )
    , IETimerConsumer           ( )

    , mMsTimeout                ( timeout )
    , mTimer                    ( static_cast<IETimerConsumer &>(self()), timerName(owner) )
    , mClient                   ( )
{
}

bool PublicHelloWorldClient::serviceConnected(bool isConnected, ProxyBase & proxy)
{
    TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_serviceConnected);
    TRACE_DBG("Client [ %p ] - [ %s ] is [ %s ]"
                , this
                , ProxyAddress::convAddressToPath(proxy.getProxyAddress()).getString()
                , isConnected ? "CONNECTED" : "DISCONNECTED");

    bool result{ false };
    if ( PublicHelloWorldClientBase::serviceConnected(isConnected, proxy) )
    {
        // Reset the ID here. Otherwise, it keeps old value when service connection lost.
        mClient.crID = 0;
        if (isConnected )
        {
            TRACE_DBG("Client [ %p ]-[ %s ] sends request to register", this, mTimer.getName().getString());
            requestRegister(mTimer.getName(), proxy.getProxyAddress(), proxy.getProxyDispatcherThread().getName(), Process::getInstance().getAppName());
        }
        else
        {
            mTimer.stopTimer( );
        }

        result = true;
    }
    else if (SystemShutdownClientBase::serviceConnected(isConnected, proxy))
    {
        TRACE_DBG("Client [ %p ]-[ %s ]subscribes on service unavailable and service state update messages", this, mTimer.getName().getString());
        notifyOnServiceStateUpdate( isConnected );

        result = true;
    }

    return result;
}

void PublicHelloWorldClient::responseRegister( const NEPublicHelloWorld::sClientRegister & client )
{
    TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_responseRegister);

    if (client.crID != 0)
    {
        ASSERT(client.crName == mTimer.getName());

        TRACE_INFO("The client [ %s ] is registered and got ID [ %d ], can use services", client.crName.getString(), client.crID);
        mClient = client;
        if ( (mClient.crID != 0) && (mTimer.isActive( ) == false) )
        {
            mTimer.startTimer( mMsTimeout );
        }
    }
    else
    {
        TRACE_ERR("Failed to register, cannot use service!");
    }
}

void PublicHelloWorldClient::responseHelloWorld(unsigned int clientID)
{
    TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_responseHelloWorld);
    if ( (clientID != 0) && (mClient.crID == clientID) )
    {
        TRACE_DBG("Client [ %s ] SUCCEEDED to make output on remote service [ %s ]", mClient.crName.getString(), PublicHelloWorldClientBase::getServiceRole().getString());
    }
    else
    {
        TRACE_ERR("Client [ %s ] FAILED to make output on remote service [ %s ]", mClient.crName.getString(), PublicHelloWorldClientBase::getServiceRole().getString());
        mTimer.stopTimer();
        requestUnregister(mClient);
        mClient.crID = 0;
    }
}

void PublicHelloWorldClient::onServiceStateUpdate( NESystemShutdown::eServiceState ServiceState, NEService::eDataStateType state )
{
    TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_onServiceStateUpdate);
    TRACE_DBG("Service state updated [ %s ], data state [ %s ]", NESystemShutdown::getString(ServiceState), NEService::getString(state));

    if (state == NEService::eDataStateType::DataIsOK)
    {
        if (ServiceState == NESystemShutdown::eServiceState::ServiceShutdown)
        {
            mTimer.stopTimer();
            if (mClient.crID != 0)
            {
                requestUnregister(mClient);
                mClient.crID = 0;
            }

            requestSystemShutdown( );
            Application::signalAppQuit( );
        }
        else if ( (mClient.crID != 0) && (mTimer.isActive() == false) )
        {
            mTimer.startTimer(mMsTimeout);
        }
    }
}

void PublicHelloWorldClient::processTimer(Timer & timer)
{
    TRACE_SCOPE(examples_13_pubmesh_common_PublicHelloWorldClient_processTimer);
    ASSERT(&timer == &mTimer);

    TRACE_DBG("Timer [ %s ] of client ID [ %d ] has expired, send request to output message.", timer.getName().getString(), mClient.crID);

    NEService::eDataStateType dataState = NEService::eDataStateType::DataIsInvalid;
    NESystemShutdown::eServiceState serviceState = getServiceState( dataState );
    if ( (serviceState == NESystemShutdown::eServiceState::ServiceReady) && (dataState == NEService::eDataStateType::DataIsOK))
    {
        requestHelloWorld(mClient.crID);
    }
    else
    {
        TRACE_WARN("Ignored sending request, the service state is not active anymore, service state [ %s ], data state [ %s ]"
                        , NESystemShutdown::getString(serviceState)
                        , NEService::getString(dataState));
    }
}

inline String PublicHelloWorldClient::timerName( Component & owner ) const
{
    String result;
    result.append("Public_")
          .append( owner.getRoleName( ) )
          .append(NECommon::DEFAULT_SPECIAL_CHAR)
          .append(PublicHelloWorldClientBase::getServiceName());

    return result;
}
