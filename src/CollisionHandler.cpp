#include "CollisionHandler.hpp"

void CollisionHandler::onEvent(Event event)
{
    if(event.getEvent() == event.type::COLLISION || event.getEventInfo() == "COLLISION"){
        event.actor->velocityY = 0;
        event.actor->isGrounded = true;
    }
}
