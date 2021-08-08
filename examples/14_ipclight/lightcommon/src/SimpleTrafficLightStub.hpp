//////////////////////////////////////////////////////////////////////////
// Begin generate lightcommon/src/SimpleTrafficLightStub.hpp file
//////////////////////////////////////////////////////////////////////////
#ifndef  LIGHTCOMMON_SRC_SIMPLETRAFFICLIGHTSTUB_HPP
#define  LIGHTCOMMON_SRC_SIMPLETRAFFICLIGHTSTUB_HPP
/************************************************************************
 * (c) copyright    2021
 *                  Create by AREG SDK code generator tool from source SimpleTrafficLight.
 * Generated at     03.08.2021  11:01:20 GMT+02:00 
 ************************************************************************/

/************************************************************************
 * \file            lightcommon/src/SimpleTrafficLightStub.hpp
 * \ingroup         SimpleTrafficLight Service Interface
 * \brief           This is an automatic generated code of SimpleTrafficLight Service Interface Stub class declaration.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "lightcommon/src/NESimpleTrafficLight.hpp"
#include "areg/component/StubBase.hpp"

/************************************************************************
 * Dependencies
 ************************************************************************/

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLightStub class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       SimpleTrafficLight Service Interface Stub object.
 *              Instantiated per every server component, which differ by
 *              role name of component.
 *
 *              The simple traffic light that triggers timeout and changes traffic light state.
 **/
class SimpleTrafficLightStub   : protected  StubBase
{
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor. Protected
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Initializes Stub by given component object, which should be already instantiated.
     * \param   masterComp  The master component object, which is initializing service Stub.
     * \note    Before constructor is called, the instance of Component must be already initialized.
     **/
    SimpleTrafficLightStub( Component & masterComp );
    /**
     * \brief   Destructor.
     **/
    virtual ~SimpleTrafficLightStub( void );
    
//////////////////////////////////////////////////////////////////////////
// Operations
//////////////////////////////////////////////////////////////////////////
public:
    /**
     * \brief   Unlocks and cancels all requests, excepts those which were manually unlocked.
     *          This call will send cancel error message (NEService::RESULT_REQUEST_CANCELED) to all clients waiting for response.
     **/
    void unlockAllRequests( void );

//////////////////////////////////////////////////////////////////////////
// Start Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLight Interface Attributes
//////////////////////////////////////////////////////////////////////////
 public:
/************************************************************************
 * Attribute SouthNorth functions
 ************************************************************************/

    /**
     * \brief   Returns true if SouthNorth attribute is valid
     **/
    inline bool isSouthNorthValid( void ) const;
    /**
     * \brief   Invalidates SouthNorth attribute and
     *          sends error update notification message to clients.
     **/
    inline void invalidateSouthNorth( void );
    /**
     * \brief   Force to send SouthNorth attribute notification message to all clients.
     *          The method can be called when the attribute is updated not via set-method.
     **/
     inline void notifySouthNorthUpdated( void );
    /**
     * \brief   Set SouthNorth attribute value and send notification event to all clients.
     *          The notification is sent only when value is updated or validated.
     * \param   newValue    New value of SouthNorth attribute to set.
     *                      The SouthNorth attribute description:
     *                      The light state of south-north traffic.
     **/
    virtual void setSouthNorth( const NESimpleTrafficLight::eTrafficLight & newValue );
    /**
     * \brief   Returns the value of SouthNorth attribute.
     *          The SouthNorth attribute description:
     *          The light state of south-north traffic.
     **/
    inline const NESimpleTrafficLight::eTrafficLight & getSouthNorth( void ) const;
    inline NESimpleTrafficLight::eTrafficLight & getSouthNorth( void );

/************************************************************************
 * Attribute EastWest functions
 ************************************************************************/

    /**
     * \brief   Returns true if EastWest attribute is valid
     **/
    inline bool isEastWestValid( void ) const;
    /**
     * \brief   Invalidates EastWest attribute and
     *          sends error update notification message to clients.
     **/
    inline void invalidateEastWest( void );
    /**
     * \brief   Force to send EastWest attribute notification message to all clients.
     *          The method can be called when the attribute is updated not via set-method.
     **/
     inline void notifyEastWestUpdated( void );
    /**
     * \brief   Set EastWest attribute value and send notification event to all clients.
     *          The notification is sent only when value is updated or validated.
     * \param   newValue    New value of EastWest attribute to set.
     *                      The EastWest attribute description:
     *                      The light state of east-west traffic.
     **/
    virtual void setEastWest( const NESimpleTrafficLight::eTrafficLight & newValue );
    /**
     * \brief   Returns the value of EastWest attribute.
     *          The EastWest attribute description:
     *          The light state of east-west traffic.
     **/
    inline const NESimpleTrafficLight::eTrafficLight & getEastWest( void ) const;
    inline NESimpleTrafficLight::eTrafficLight & getEastWest( void );

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLight Interface Requests
//////////////////////////////////////////////////////////////////////////
public:

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLight Interface Responses
//////////////////////////////////////////////////////////////////////////
public:

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLight Interface Broadcasts
//////////////////////////////////////////////////////////////////////////
public:

    /**
     * \brief   Broadcast call.
     *          Triggered when the light state of traffic is changed.
     * \param   SouthNorth  The light state of south-north traffic.
     * \param   EastWest    The light state of east-west traffic.
     **/
    virtual void broadcastLightChanged( NESimpleTrafficLight::eTrafficLight SouthNorth, NESimpleTrafficLight::eTrafficLight EastWest );

//////////////////////////////////////////////////////////////////////////
// End Service Interface operations / attributes and overrides declaration
//////////////////////////////////////////////////////////////////////////

//////////////////////////////////////////////////////////////////////////
//  Operations.
//////////////////////////////////////////////////////////////////////////
public:
/************************************************************************/
// StubBase overrides. Public methods.
/************************************************************************/

    /**
     * \brief   Sends update notification message to all clients. 
     *          This method can be called manually to send update 
     *          notification message after updating attribute value.
     *
     * \param   msgId   The attribute message ID to notify clients.
     **/
    virtual void sendNotification( unsigned int msgId );

    /**
     * \brief   Sends error message to clients.
     *          If message ID is a request, it should send result NEService::RESULT_REQUEST_ERROR or NEService::RESULT_REQUEST_CANCELED, depending on msgCancel flag.
     *          If message ID is a response, it should send result NEService::RESULT_INVALID.
     *          If message ID is an attribute, it should send result NEService::RESULT_DATA_INVALID
     *          and invalidate attribute data value.
     *
     * \param   msgId       The message ID to send error message
     * \param   msgCancel   Indicates whether the request is canceled or should respond with error.
     *                      This parameter has sense only for request IDs.
     *                      It is ignored for response and attributes IDs.
     **/
    virtual void errorRequest( unsigned int msgId, bool msgCancel );

protected:
/************************************************************************/
// StubBase overrides. Triggered by Component on startup.
/************************************************************************/

    /**
     * \brief   This function is triggered by Component when starts up.
     *          Overwrite this method and set appropriate request and
     *          attribute update notification event listeners here
     * \param   holder  The holder component of service interface of Stub,
     *                  which started up.
     **/
    virtual void startupServiceInterface( Component & holder );

    /**
     * \brief   This function is triggered by Component when shuts down.
     *          Overwrite this method to remove listeners and stub cleanup
     * \param   holder  The holder component of service interface of Stub,
     *                  which shuts down.
     **/
    virtual void shutdownServiceIntrface ( Component & holder );

/************************************************************************/
// StubBase overrides. Protected methods 
/************************************************************************/

    /**
     * \brief   Creates Response event object to pass of client.
     * \param   proxy   The address of proxy object to send response event
     * \param   msgId   The message ID to send to client
     * \param   result  The result of message
     * \param   data    The buffer of data to send to client. Can be Invalid buffer.
     * \return  Returns valid pointer to Response event object
     **/
    virtual ResponseEvent * createResponseEvent( const ProxyAddress & proxy, unsigned int msgId, NEService::eResultType result, const EventDataStream & data ) const;

    /**
     * \brief   Overwrite method to create remote service request event from streaming object for 
     *          further dispatching by stub.
     * \param   stream  Streaming object, which contains event data.
     * \return  If operation succeeds, returns valid pointer to Service Request event object.
     *          Otherwise, it returns NULL.
     **/
    virtual RemoteRequestEvent * createRemoteRequestEvent( const IEInStream & stream ) const;

    /**
     * \brief   Overwrite method to create remote notify request event from streaming object for 
     *          further dispatching by stub.
     * \param   stream  Streaming object, which contains event data.
     * \return  If operation succeeds, returns valid pointer to Service Request event object.
     *          Otherwise, it returns NULL.
     **/
    virtual RemoteNotifyRequestEvent * createRemoteNotifyRequestEvent( const IEInStream & stream ) const;

private:
/************************************************************************/
// IEComponentConsumer interface overrides.
/************************************************************************/

    /**
     * \brief   Triggered to process service request event.
     *          Overwrite method to process every service request event.
     * \param   eventElem   Service Request Event object, contains request
     *                      call ID and parameters.
     **/
    virtual void processRequestEvent( ServiceRequestEvent & eventElem );

    /**
     * \brief   Triggered to process attribute update notification event.
     *          Override method to process request to get attribute value and
     *          process notification request of attribute update.
     * \param   eventElem   Service Request Event object, contains attribute ID.
     **/
    virtual void processAttributeEvent( ServiceRequestEvent & eventElem );

//////////////////////////////////////////////////////////////////////////
// Hidden function calls
//////////////////////////////////////////////////////////////////////////
private:
    /**
     * \brief   Returns reference to SimpleTrafficLightStub object
     **/
    inline SimpleTrafficLightStub & self( void );

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
/************************************************************************
 * Attributes
 ************************************************************************/

    /**
     * \brief   Attribute SouthNorth
     *          The light state of south-north traffic.
     **/
    NESimpleTrafficLight::eTrafficLight mSouthNorth;
    /**
     * \brief   Attribute SouthNorth data validation state.
     **/
    NEService::eDataStateType           mSouthNorthState;

    /**
     * \brief   Attribute EastWest
     *          The light state of east-west traffic.
     **/
    NESimpleTrafficLight::eTrafficLight mEastWest;
    /**
     * \brief   Attribute EastWest data validation state.
     **/
    NEService::eDataStateType           mEastWestState;

//////////////////////////////////////////////////////////////////////////
// Forbidden calls
//////////////////////////////////////////////////////////////////////////
private:
    SimpleTrafficLightStub( void );
    SimpleTrafficLightStub( const SimpleTrafficLightStub & /* src */ );
    const SimpleTrafficLightStub & operator = ( const SimpleTrafficLightStub & /* src */ );
};

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLightStub class inline functions implementation
//////////////////////////////////////////////////////////////////////////

inline SimpleTrafficLightStub & SimpleTrafficLightStub::self( void )
{
    return (*this);
}

/************************************************************************
 * \brief   SouthNorth attribute functions
 ************************************************************************/
 
inline bool SimpleTrafficLightStub::isSouthNorthValid( void ) const
{
    return (mSouthNorthState  == NEService::DATA_OK);
}

inline void SimpleTrafficLightStub::invalidateSouthNorth( void )
{
    errorRequest( static_cast<unsigned int>(NESimpleTrafficLight::MSG_ID_SouthNorth), false );
}

inline void SimpleTrafficLightStub::notifySouthNorthUpdated( void )
{
    sendNotification( NESimpleTrafficLight::MSG_ID_SouthNorth );
}

inline const NESimpleTrafficLight::eTrafficLight & SimpleTrafficLightStub::getSouthNorth( void ) const
{
    return mSouthNorth;
}

inline NESimpleTrafficLight::eTrafficLight & SimpleTrafficLightStub::getSouthNorth( void )
{
    return mSouthNorth;
}

/************************************************************************
 * \brief   EastWest attribute functions
 ************************************************************************/
 
inline bool SimpleTrafficLightStub::isEastWestValid( void ) const
{
    return (mEastWestState  == NEService::DATA_OK);
}

inline void SimpleTrafficLightStub::invalidateEastWest( void )
{
    errorRequest( static_cast<unsigned int>(NESimpleTrafficLight::MSG_ID_EastWest), false );
}

inline void SimpleTrafficLightStub::notifyEastWestUpdated( void )
{
    sendNotification( NESimpleTrafficLight::MSG_ID_EastWest );
}

inline const NESimpleTrafficLight::eTrafficLight & SimpleTrafficLightStub::getEastWest( void ) const
{
    return mEastWest;
}

inline NESimpleTrafficLight::eTrafficLight & SimpleTrafficLightStub::getEastWest( void )
{
    return mEastWest;
}

//////////////////////////////////////////////////////////////////////////
// SimpleTrafficLightStub class declaration End
//////////////////////////////////////////////////////////////////////////

#endif   // LIGHTCOMMON_SRC_SIMPLETRAFFICLIGHTSTUB_HPP

//////////////////////////////////////////////////////////////////////////
// End generate lightcommon/src/SimpleTrafficLightStub.hpp file
//////////////////////////////////////////////////////////////////////////
