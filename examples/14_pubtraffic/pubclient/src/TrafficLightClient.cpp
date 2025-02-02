/************************************************************************
 * \file        pubclient/src/TrafficLightClient.cpp
 * \ingroup     AREG Asynchronous Event-Driven Communication Framework examples
 * \author      Artak Avetyan
 * \brief       Collection of AREG SDK examples.
 *              This file contains simple implementation of service client.
 ************************************************************************/
/************************************************************************
 * Include files.
 ************************************************************************/

#include "pubclient/src/TrafficLightClient.hpp"
#include "areg/appbase/Application.hpp"

Component * TrafficLightClient::CreateComponent(const NERegistry::ComponentEntry & entry, ComponentThread & owner)
{
    return DEBUG_NEW TrafficLightClient(entry, owner);
}

void TrafficLightClient::DeleteComponent(Component & compObject, const NERegistry::ComponentEntry & entry)
{
    delete (&compObject);
}

TrafficLightClient::TrafficLightClient(const NERegistry::ComponentEntry & entry, ComponentThread & owner)
    : Component                     ( entry, owner )
    , SimpleTrafficLightClientBase  ( entry.mDependencyServices[0], static_cast<Component &>(self()) )

    , mTrafficDirection             ( static_cast<NECommon::eTrafficDirection>(entry.getComponentData().alignInt.mElement) )
{
}

bool TrafficLightClient::serviceConnected(bool isConnected, ProxyBase & proxy)
{
    bool result{ false };
    if ( SimpleTrafficLightClientBase::serviceConnected( isConnected, proxy ) )
    {
        if ( mTrafficDirection == NECommon::eTrafficDirection::DirectionSouthNorth )
        {
            notifyOnSouthNorthUpdate( isConnected );
        }
        else
        {
            notifyOnEastWestUpdate( isConnected );
        }

        if ( isConnected == false )
        {
            Application::signalAppQuit( );
        }

        result = true;
    }

    return result;
}

void TrafficLightClient::onSouthNorthUpdate(NESimpleTrafficLight::eTrafficLight SouthNorth, NEService::eDataStateType state)
{
    if (state == NEService::eDataStateType::DataIsOK)
    {
        outputState(SouthNorth);
    }
}

void TrafficLightClient::onEastWestUpdate(NESimpleTrafficLight::eTrafficLight EastWest, NEService::eDataStateType state)
{
    if (state == NEService::eDataStateType::DataIsOK)
    {
        outputState(EastWest);
    }
}

inline void TrafficLightClient::outputState(NESimpleTrafficLight::eTrafficLight lightState)
{
    switch (lightState)
    {
    case NESimpleTrafficLight::eTrafficLight::LightRed:
        std::cout << "Light: RED ..." << std::endl;
        break;
    case NESimpleTrafficLight::eTrafficLight::LightYellow:
        std::cout << "Light: Yellow ..." << std::endl;
        break;
    case NESimpleTrafficLight::eTrafficLight::LightGreen:
        std::cout << "Light: GREEN ..." << std::endl;
        break;
    case NESimpleTrafficLight::eTrafficLight::LightOff:
    default:
        std::cout << "Light: OFF ..." << std::endl;
        break;
    }
}
