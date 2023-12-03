#ifndef EVENT_HPP
#define EVENT_HPP

#include "Actor.hpp"
#include "Object.hpp"
#include "DeathZone.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include "Bound.hpp"
#include <zmq.hpp>
#include <vector>
#include <iostream>
#include <iterator>
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class Event
{
public:
    enum type
    {
        COLLISION,
        DEATH,
        SPAWN,
        USERINPUT
    };

    Event(type event, std::string eventInformation, int64_t priority, Actor *actor);
    Event(type event, std::string eventInformation, int64_t priority, Actor *actor, bool inputs[], Timeline *timeline);

    std::string getEventInfo();
    type getEvent();
    int64_t getPriority();
    void setPriority(int64_t priority);
    Timeline *timeline;
    int boolsize = 15;
    bool booleanArray[15];
    Actor *actor;

private:
    type event;
    int64_t priority;
    std::string eventInformation;

};

#endif