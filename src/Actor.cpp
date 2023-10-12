#include "Actor.hpp"

Actor::Actor(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath, int id)
    : Object(*shape, position, texturePath) {
        this->id = id;
}

int Actor::getId(){
    return id;
}

