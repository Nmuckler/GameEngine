#ifndef EVENTHANDLER_HPP
#define EVENTHANDLER_HPP

#include "Event.hpp"

class EventHandler {
public:
    virtual void onEvent(Event event) = 0; // Pure virtual function
    virtual ~EventHandler() {} // Virtual destructor for proper polymorphic destruction
};

#endif