#include "Actor.hpp"

Actor::Actor(sf::Shape *shape, sf::Vector2f position, const std::string &texturePath, int id)
    : Object(*shape, position, texturePath)
{
    this->id = id;
    spawnX = position.x;
    spawnY = position.y;
    isGrounded = false;
}

int Actor::getId()
{
    return id;
}

void Actor::setSpawn(int x, int y)
{
    spawnX = x;
    spawnY = y;
}

void Actor::respawn()
{
    printf("I just died\n");
    this->positionX = spawnX;
    this->positionY = spawnY;

}
