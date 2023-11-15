#ifndef EVENTMANAGER_HPP
#define EVENTMANAGER_HPP

#include "Actor.hpp"
#include "Object.hpp"
#include "DeathZone.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include "Bound.hpp"
#include "Event.hpp"
#include "EventHandler.hpp"
#include "CollisionHandler.hpp"
#include "UserInputHandler.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <mutex>
#include <string>
#include <sstream>
#include <queue>

#include <list>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class EventManager
{
public:
    EventManager();
    static EventManager *getInstance();
    void initialize();

    std::mutex lock;

    struct Compare
    {
        bool operator()(Event* &e1, Event* &e2) const
        {
            // Lower priority is considered "greater" for std::priority_queue
            return e1->getPriority() > e2->getPriority();
        }
    };

    void registerHandler(Event::type eventType, EventHandler *handler);
    void deregisterHandler(Event::type eventType, EventHandler *handler);
    void raise(Event *e);
    void processEvents();
    std::priority_queue<Event *, std::vector<Event *>, Compare> pq;

private:
    std::map<Event::type, std::list<EventHandler *>> handlers;
    std::vector<Event *> raised_events;
    Timeline timeline = Timeline();
    static EventManager *singleton;

    // handlers
    CollisionHandler C_Handler = CollisionHandler();
    UserInputHandler UI_Handler = UserInputHandler();
};

#endif
