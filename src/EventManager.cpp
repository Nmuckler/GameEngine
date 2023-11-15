#include "EventManager.hpp"

EventManager *EventManager::singleton = nullptr;

EventManager::EventManager()
{
}

EventManager *EventManager::getInstance()
{
    if (singleton == nullptr)
    {
        singleton = new EventManager();
    }
    return singleton;
}

void EventManager::initialize()
{
    registerHandler(Event::COLLISION, &C_Handler);
}

void EventManager::registerHandler(Event::type eventType, EventHandler *handler)
{
    handlers[eventType].push_back(handler);
}

void EventManager::deregisterHandler(Event::type eventType, EventHandler *handler)
{
    auto it = handlers.find(eventType);
    if (it != handlers.end())
    {
        it->second.remove(handler);
    }
}

void EventManager::raise(Event *e)
{
    pq.push(e);
}

void EventManager::processEvents()
{
    calledrespawn = false;


    while (!pq.empty())
    {
        Event *e = pq.top();

        if (e->getPriority() <= timeline.getTime())
        {
            pq.pop();
            if (e->getEventInfo() == "COLLISION")
                C_Handler.onEvent(*e);
            else if (e->getEventInfo() == "USERINPUT")
                UI_Handler.onEvent(*e);
            else if (e->getEventInfo() == "SPAWN")
            {
                S_Handler.onEvent(*e);
                calledrespawn = true;
            }
            else if (e->getEventInfo() == "DEATH")
                D_Handler.onEvent(*e);
        }
        else
        {
            // No more events with priority less than or equal to the current time
            break;
        }
    }
}
