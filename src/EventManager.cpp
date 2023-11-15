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
    std::vector<Event*> tempVector;

    while (!pq.empty())
    {
        Event *e = pq.top();

        if (e->getPriority() <= timeline.getTime())
        {
            pq.pop();
            C_Handler.onEvent(*e);
        }
        else
        {
            // No more events with priority less than or equal to the current time
            break;
        }

        tempVector.push_back(e);
    }

    // Restore the processed events back to the priority queue
    for (const auto &e : tempVector)
    {
        pq.push(e);
    }
}
