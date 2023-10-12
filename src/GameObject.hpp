#ifndef GAMEOBJECT_HPP
#define GAMEOBJECT_HPP

#include "Object.hpp"

class GameObject: public Object {
public:
    GameObject(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath);

    void hoverX(int x1, int x2, float speed);

    void hoverY(int y1, int y2, float speed);

private:
    bool moveX;
    bool moveY;
};

#endif