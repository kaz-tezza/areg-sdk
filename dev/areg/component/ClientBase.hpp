#ifndef AREG_COMPONENT_CLIENTBASE_HPP
#define AREG_COMPONENT_CLIENTBASE_HPP
/************************************************************************
 * \file        areg/component/ClientBase.hpp
 * \ingroup     AREG SDK, Asynchronous Event Generator Software Development Kit 
 * \author      Artak Avetyan (mailto:artak@aregtech.com)
 * \brief       AREG Platform, Client Base class.
 *              Base class for all client objects. In case of appropriate
 *              function call failure, it will generate error message and
 *              trigger assertion.
 *
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/
#include "areg/base/GEGlobal.h"
#include "areg/base/String.hpp"

//////////////////////////////////////////////////////////////////////////
// ClientBase class declaration
//////////////////////////////////////////////////////////////////////////
/**
 * \brief       Base class for all clients. It generates error messages
 *              and trigger assertions (valid for Debug version) in case
 *              either one of responses and updates is not implemented or
 *              appropriate error handling function is not overwritten.
 **/
class AREG_API ClientBase
{
    static const char * const   UNKNOWN_NAME    /*= "<Unknown>"*/;
//////////////////////////////////////////////////////////////////////////
// Constructor / Destructor. Protected members.
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief   Default constructor. Protected, should be instantiated by child classes.
     **/
    ClientBase( const char * clientName );
    /**
     * \brief   Destructor.
     **/
    virtual ~ClientBase( void );

//////////////////////////////////////////////////////////////////////////
// Operations. Protected.
//////////////////////////////////////////////////////////////////////////
protected:
    /**
     * \brief	This function is triggered in child class if it has no
     *          implementation of certain request call failure.
     * \param	requestId	The specific function request ID that cause failure
     **/
    void requestFailedNotImplemented( unsigned int requestId ) const;

    /**
     * \brief	This function is triggered in child class if it has no
     *          implementation of certain response
     * \param	responseId	The specific response ID that has no implementation.
     **/
    void responseNotImplemented( unsigned int responseId ) const;

    /**
     * \brief	This function is triggered in child class if it has no
     *          implementation of certain broadcast method of service interface
     * \param	broadcastId The specific response ID that has no implementation.
     **/
    void broadcastNotImplemented( unsigned int broadcastId) const;

    /**
     * \brief	This function is triggered in child class if response was not
     *          handled, i.e. could not find response function.
     * \param	responseId	The specific response ID, which was not handled in client.
     **/
    void responseInvalidNotImpelemnted( unsigned int responseId) const;

    /**
     * \brief	This function is triggered in child class if client assigned
     *          listener on certain attribute update, but has no on update 
     *          function implementation, i.e. the update ID was not handled.
     * \param	updateId	The specific ID of update message, which was not handled.
     **/
    void onUpdateNotImplemented( unsigned int updateId) const;

    /**
     * \brief   Returns given client name
     **/
    inline const String & getClientName( void ) const;

//////////////////////////////////////////////////////////////////////////
// Member variables
//////////////////////////////////////////////////////////////////////////
private:
    const String    mClientName;    //!< The name of client given by derived parent
};

inline const String & ClientBase::getClientName( void ) const
{
    return mClientName;
}

#endif  // AREG_COMPONENT_CLIENTBASE_HPP
