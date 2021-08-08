#ifndef  LIGHTCOMMON_SRC_NESIMPLELIGHTCOMMON_HPP
#define  LIGHTCOMMON_SRC_NESIMPLELIGHTCOMMON_HPP

/************************************************************************
 * \file            lightcommon/src/NESimpleLightCommon.hpp
 * \ingroup         SimpleTrafficLight Service Interface
 * \brief           SimpleTrafficLight Service Interface common constants.
 ************************************************************************/

/************************************************************************
 * Include files
 ************************************************************************/
#include "areg/base/GEGlobal.h"

/**
 * \brief   Traffic light common names.
 **/
namespace NESimpleLightCommon
{
    /**
     * \brief   The service name of simple traffic light controller.
     **/
    const char * const  SimpleLightControllerName       = "SimpleLightController";

    /**
     * \brief   The prefix of simple traffic light client.
     **/
    const char * const  SimpleLightClientNamePrefix     = "SimpleLight_";

    /**
     * \brief   These are symbolic directions of traffics.
     **/
    typedef enum E_TrafficDirection
    {
          DirectionUnknown      //!< The direction is unknown
        , DirectionSouthNorth   //!< The direction is South-North
        , DirectionEastWest     //!< The direction is East-West.

    } eTrafficDirection;
}

#endif // LIGHTCOMMON_SRC_NESIMPLELIGHTCOMMON_HPP
