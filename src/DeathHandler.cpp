#include "DeathHandler.hpp"

void DeathHandler::onEvent(Event event)
{
    if(event.getEvent() == event.type::DEATH || event.getEventInfo() == "DEATH"){
        event.actor->kill();
    }
}
