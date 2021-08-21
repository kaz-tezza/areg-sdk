//////////////////////////////////////////////////////////////////////////
// Begin generate generated/src/private/SystemShutdownClientBase.cpp file
//////////////////////////////////////////////////////////////////////////
/************************************************************************
 * (c) copyright    2021
 *                  Create by AREG SDK code generator tool from source SystemShutdown.
 * Generated at     11.08.2021  13:17:34 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            generated/src/private/SystemShutdownClientBase.cpp
 * \ingroup         SystemShutdown Service Interface
 * \brief           This is an automatic generated code of SystemShutdown Service Interface Client base class implementation.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "generated/src/SystemShutdownClientBase.hpp"
#include "areg/component/ComponentThread.hpp"
#include "areg/component/Component.hpp"
#include "areg/trace/GETrace.h"

/************************************************************************
 * Request error handling initialization
 ************************************************************************/
namespace NESystemShutdown
{
    /**
     * \brief   Initialize request failure function pointers to make error handling
     **/
    typedef void (SystemShutdownClientBase::* FuncRequestFailure) ( NEService::eResultType );
#ifdef  _DEBUG
    static FuncRequestFailure failureFunctions = static_cast<FuncRequestFailure>(NULL);
#endif  // _DEBUG
}

//////////////////////////////////////////////////////////////////////////
// SystemShutdownClientBase class implementation
//////////////////////////////////////////////////////////////////////////

/************************************************************************
 * Constructor / Destructor
 ************************************************************************/

SystemShutdownClientBase::SystemShutdownClientBase( const char * roleName, const char * ownerThread /*= static_cast<const char *>(NULL)*/ )
    : IEProxyListener   ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( SystemShutdownProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), ownerThread) )
{
    ; // do nothing
}

SystemShutdownClientBase::SystemShutdownClientBase( const char * roleName, DispatcherThread & ownerThread )
    : IEProxyListener   ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( SystemShutdownProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), ownerThread) )
{
    ; // do nothing
}

SystemShutdownClientBase::SystemShutdownClientBase( const char* roleName, Component & owner )
    : IEProxyListener   ( )

    , mIsConnected      ( false )
    , mCurrSequenceNr   ( 0 )
    , mProxy            ( SystemShutdownProxy::createProxy(roleName, static_cast<IEProxyListener &>(self()), owner.getMasterThread()) )
{
    ; // do nothing
}

SystemShutdownClientBase::~SystemShutdownClientBase( void )
{
    if (mProxy != NULL)
    {
        mProxy->clearAllNotifications( static_cast<IENotificationEventConsumer &>(self()) );
        mProxy->freeProxy( static_cast<IEProxyListener &>(self()) );
        mProxy  = NULL;
    }
    
    mIsConnected= false;
}

/************************************************************************
 * Operations
 ************************************************************************/

bool SystemShutdownClientBase::recreateProxy( void )
{
    bool result         = false;
    if (mProxy != NULL)
    {
        String roleName   = mProxy->getProxyAddress().getRoleName();
        String threadName = mProxy->getProxyAddress().getThread();
        if ( roleName.isEmpty() == false )
        {
            SystemShutdownProxy * newProxy = SystemShutdownProxy::createProxy(roleName.getString(), static_cast<IEProxyListener &>(self()), threadName.getString());
            if (newProxy != NULL)
            {
                mProxy->clearAllNotifications( static_cast<IENotificationEventConsumer &>(self()) );
                mProxy->freeProxy (static_cast<IEProxyListener &>(self()) );
                mProxy = newProxy;
                result = true;
            }
        }    
    }
    return result;
}

DispatcherThread * SystemShutdownClientBase::getDispatcherThread( void )
{
    return ( mProxy != static_cast<SystemShutdownProxy *>(NULL) ? &(mProxy->getProxyDispatcherThread()) : static_cast<DispatcherThread *>(NULL) );
}

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_serviceConnected);
bool SystemShutdownClientBase::serviceConnected( bool isConnected, ProxyBase & proxy )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_serviceConnected);
    
    bool result = false;
    if(mProxy == &proxy)
    {
        TRACE_DBG("Client [ %p ]: The Service [ %s ] with Role Name [ %s ] is [ %s ]"
                 , this
                 , proxy.getProxyAddress().getServiceName().getString()
                 , proxy.getProxyAddress().getRoleName().getString()
                 , isConnected ? "CONNECTED" : "DISCONNECTED");
        
        mIsConnected= isConnected;
        result      = true;
    }
    
    return result;
}

void SystemShutdownClientBase::notifyOn( NESystemShutdown::eMessageIDs msgId, bool notify, bool always /* = false */ )
{
    if (notify)
        mProxy->setNotification(msgId, static_cast<IENotificationEventConsumer &>(self()), always);
    else
        mProxy->clearNotification(msgId, static_cast<IENotificationEventConsumer &>(self()));
}

/************************************************************************
 * Event processing
 ************************************************************************/

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_processNotificationEvent);
void SystemShutdownClientBase::processNotificationEvent( NotificationEvent & eventElem )
{
    const NotificationEventData & data  = static_cast<const NotificationEvent &>(eventElem).getData();
    NEService::eResultType result       = data.getNotifyType();
    NESystemShutdown::eMessageIDs msgId   = static_cast<NESystemShutdown::eMessageIDs>(data.getNotifyId());
    mCurrSequenceNr = data.getSequenceNr();

    switch (result)
    {
    case NEService::RESULT_REQUEST_BUSY:
    case NEService::RESULT_REQUEST_ERROR:
    case NEService::RESULT_REQUEST_CANCELED:
        {
        /************************************************************************
         * Trigger request error handling if request failed.
         ************************************************************************/
            SystemShutdownClientBase::requestFailed(msgId, result);
        }
        break;

    case NEService::RESULT_INVALID:
        {
        /************************************************************************
         * Trigger invalid response / broadcast handling. May happen when remove notification 
         ************************************************************************/
            SystemShutdownClientBase::invalidResponse(msgId);
        }
        break;

    case NEService::RESULT_DATA_OK:
    case NEService::RESULT_DATA_INVALID:
        {
            NEService::eDataStateType dataState;
            switch (msgId)
            {
        /************************************************************************
         * Trigger attribute update processing
         ************************************************************************/
            case NESystemShutdown::MSG_ID_ServiceState:
                {
                    NESystemShutdown::eServiceState ServiceState = mProxy->getServiceState(dataState);
                    onServiceStateUpdate(ServiceState, dataState);
                }
                break;

            default:
                {
                    TRACE_SCOPE(generated_src_SystemShutdownClientBase_processNotificationEvent);
                    TRACE_ERR("Client object SystemShutdownClientBase of proxy [ %s ] received unexpected Attribute update notification message ID [ %d ]."
                                , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                                , msgId);
                    ASSERT(false);
                }
                break;
            }
        }
        break;

    case NEService::RESULT_OK:
        {
            switch (msgId)
            {
        /************************************************************************
         * Trigger response processing
         ************************************************************************/
        /************************************************************************
         * Trigger broadcast processing
         ************************************************************************/
            case NESystemShutdown::MSG_ID_broadcastServiceUnavailable:
                {
                    broadcastServiceUnavailable(  );
                }
                break;

            case NESystemShutdown::MSG_ID_broadcastServiceShutdown:
                {
                    broadcastServiceShutdown(  );
                }
                break;

            default:
                {
                    TRACE_SCOPE(generated_src_SystemShutdownClientBase_processNotificationEvent);
                    TRACE_ERR("Client object SystemShutdownClientBase of proxy [ %s ] received unexpected Response message ID [ %d ]."
                                , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                                , msgId);
                    ASSERT(false);
                }
                break;
            }
        }        
        break;

    default:
        {
            TRACE_SCOPE(generated_src_SystemShutdownClientBase_processNotificationEvent);
            TRACE_ERR("Client object SystemShutdownClientBase of proxy [ %s ] received unexpected Response result type [ %d ]."
                        , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                        , static_cast<int>(result));
            ASSERT(false);
        }
        break;
    }
}

/************************************************************************
 * Error handling calls
 ************************************************************************/
DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_invalidResponse);
void SystemShutdownClientBase::invalidResponse( NESystemShutdown::eMessageIDs InvalidRespId )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_invalidResponse);
    TRACE_ERR("The invalid response [ %s ] (value = [ %d ]) method of proxy [ %s ] client SystemShutdownClientBase is not implemented! Make error handling!"
                    , NESystemShutdown::getString(InvalidRespId)
                    , static_cast<unsigned int>(InvalidRespId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString() );

    ASSERT(false);
}

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_invalidRequest);
void SystemShutdownClientBase::invalidRequest( NESystemShutdown::eMessageIDs InvalidReqId )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_invalidRequest);
    TRACE_ERR("The invalid request [ %s ] (value = [ %d ]) method of proxy [ %s ] client SystemShutdownClientBase is not implemented! Make error handling!"
                    , NESystemShutdown::getString(InvalidReqId)
                    , static_cast<unsigned int>(InvalidReqId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString() );
                    
    ASSERT(false);
}

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_requestFailed);
void SystemShutdownClientBase::requestFailed( NESystemShutdown::eMessageIDs FailureMsgId, NEService::eResultType FailureReason )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_requestFailed);
    TRACE_WARN("The request [ %s ] (value = [ %d ]) method of proxy [ %s ] client SystemShutdownClientBase failed with reason [ %s ]! Implemented error handling!"
                    , NESystemShutdown::getString(FailureMsgId)
                    , static_cast<unsigned int>(FailureMsgId)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString()
                    , NEService::getString(FailureReason) );
                    
    ASSERT(NESystemShutdown::failureFunctions == NULL);
    invalidRequest( FailureMsgId );
}

/************************************************************************
 * Attribute notifications
 ************************************************************************/

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_onServiceStateUpdate);
void SystemShutdownClientBase::onServiceStateUpdate( NESystemShutdown::eServiceState /* ServiceState */, NEService::eDataStateType /* state */ )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_onServiceStateUpdate);
    TRACE_WARN("The attribute ServiceState (value = %u) update method of proxy [ %s ] client SystemShutdownClientBase is not implemented!"
                    , static_cast<unsigned int>(NESystemShutdown::MSG_ID_ServiceState)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString());
}

/************************************************************************
 * Request failure / Response and Broadcast notifications
 ************************************************************************/
 
DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_broadcastServiceUnavailable);
void SystemShutdownClientBase::broadcastServiceUnavailable( void )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_broadcastServiceUnavailable);
    TRACE_WARN("The broadcast broadcastServiceUnavailable (value = %u) method of proxy [ %s ] client SystemShutdownClientBase is not implemented!"
                    , static_cast<unsigned int>(NESystemShutdown::MSG_ID_broadcastServiceUnavailable)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString());
}

DEF_TRACE_SCOPE(generated_src_SystemShutdownClientBase_broadcastServiceShutdown);
void SystemShutdownClientBase::broadcastServiceShutdown( void )
{
    TRACE_SCOPE(generated_src_SystemShutdownClientBase_broadcastServiceShutdown);
    TRACE_WARN("The broadcast broadcastServiceShutdown (value = %u) method of proxy [ %s ] client SystemShutdownClientBase is not implemented!"
                    , static_cast<unsigned int>(NESystemShutdown::MSG_ID_broadcastServiceShutdown)
                    , ProxyAddress::convAddressToPath(mProxy->getProxyAddress()).getString());
}

//////////////////////////////////////////////////////////////////////////
// End generate generated/src/private/SystemShutdownClientBase.cpp file
//////////////////////////////////////////////////////////////////////////