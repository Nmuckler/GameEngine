#ifndef OBJECT_HPP
#define OBJECT_HPP

#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <map>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class Object
{
public:
    Object(sf::Shape &shape, sf::Vector2f position, const std::string &texturePath);

    sf::Shape &getShape();

    bool colorExists(const std::string &texturePath);

    void draw(sf::RenderWindow &window, bool drawBoundingBox);

    bool isTouching(const sf::Shape &object);

    void move(float velX, float velY);

    void moveLeft(float speed);

    void moveRight(float speed);

    void moveUp(float speed);

    void moveDown(float speed);

    void jump();

    void update(int64_t deltaT);

    void update(int64_t deltaT, bool grounded);

    const std::map<std::string, sf::Color> colorMap = {
        {"Black", sf::Color::Black},
        {"White", sf::Color::White},
        {"Red", sf::Color::Red},
        {"Green", sf::Color::Green},
        {"Blue", sf::Color::Blue},
        {"Yellow", sf::Color::Yellow},
        {"Magenta", sf::Color::Magenta},
        {"Cyan", sf::Color::Cyan},
        {"Transparent", sf::Color::Transparent}};

    sf::Shape &shape;
    sf::Texture texture;
    float positionX, positionY;
    float velocityX, velocityY;
    float gravity = 0.1f;
};

#endif