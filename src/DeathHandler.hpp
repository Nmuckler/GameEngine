#ifndef DEATHHANDLER_HPP
#define DEATHHANDLER_HPP

#include "EventHandler.hpp"

class DeathHandler: public EventHandler {
public:
    void onEvent(Event event);
    
private:
    
};

#endif