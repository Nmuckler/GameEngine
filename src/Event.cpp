#include "Event.hpp"

Event::Event(type event, std::string eventInformation, int64_t priority, Actor *actor)
{
    if (!event && eventInformation == "")
    {
        throw std::invalid_argument("Null event you silly silly boy");
    }
    this->event = event;
    this->priority = priority;
    this->eventInformation = eventInformation;
    this->actor = actor;
    
}

Event::Event(type event, std::string eventInformation, int64_t priority, Actor *actor, bool inputs[], Timeline *timeline)
{
    if (!event)
    {
        throw std::invalid_argument("Null event");
    }
    this->event = event;
    this->priority = priority;
    this->eventInformation = eventInformation;
    this->actor = actor;


    this->timeline = timeline;
    for (int i = 0; i < boolsize; i++)
        booleanArray[i] = inputs[i];
}

std::string Event::getEventInfo()
{
    return eventInformation;
}

Event::type Event::getEvent()
{
    return event;
}

int64_t Event::getPriority()
{
    return priority;
}

void Event::setPriority(int64_t priority)
{
    this->priority = priority;
}
