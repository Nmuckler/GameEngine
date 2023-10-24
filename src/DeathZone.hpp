#ifndef DEATHZONE_HPP
#define DEATHZONE_HPP


#include "Object.hpp"

class DeathZone: public Object {
public:
    DeathZone(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath);
};

#endif