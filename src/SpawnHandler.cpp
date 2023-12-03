#include "SpawnHandler.hpp"

void SpawnHandler::onEvent(Event event)
{
    if(event.getEvent() == event.type::SPAWN || event.getEventInfo() == "SPAWN"){
        event.actor->respawn();
    }
}
