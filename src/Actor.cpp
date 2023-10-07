#include "Actor.hpp"

Actor::Actor(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath)
    : Object(*shape, position, texturePath) {
}

