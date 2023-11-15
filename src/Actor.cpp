#include "Actor.hpp"

Actor::Actor(sf::Shape *shape, sf::Vector2f position, const std::string &texturePath, int id)
    : Object(*shape, position, texturePath)
{
    this->id = id;
    spawnX = position.x;
    spawnY = position.y;
    isGrounded = false;
    alive = true;
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
    printf("I just respawned\n");
    this->setColor("White");
    alive = true;
    this->positionX = spawnX;
    this->positionY = spawnY;

}

void Actor::kill()
{
    printf("I just died\n");
    alive = false;
    getShape().setFillColor(sf::Color::Transparent);
}
