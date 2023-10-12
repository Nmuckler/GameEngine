#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class Object
{
public:
    Object(sf::Shape &shape, sf::Vector2f position, const std::string &texturePath);

    sf::Shape &getShape();

    void draw(sf::RenderWindow &window, bool drawBoundingBox);

    bool isTouchingFloor(const sf::Shape &object);

    void move(float velX, float velY);

    void moveLeft(float speed);

    void moveRight(float speed);

    void moveUp(float speed);

    void moveDown(float speed);

    void jump();

    void update(int64_t deltaT);

    void update(int64_t deltaT, bool grounded);


    sf::Shape &shape;
    sf::Texture texture;
    float positionX, positionY;
    float velocityX, velocityY;
    float gravity = 0.1f;
};

#endif