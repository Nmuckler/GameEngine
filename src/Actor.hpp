#ifndef ACTOR_HPP
#define ACTOR_HPP

#include "Object.hpp"

class Actor: public Object {
public:
    Actor(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath, int id);
    int getId();
private:
    int id;
};

#endif