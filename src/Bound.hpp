#ifndef BOUND_HPP
#define BOUND_HPP


#include "Object.hpp"

class Bound: public Object {
public:
    Bound(sf::Shape* shape, sf::Vector2f position, const std::string& texturePath);
};

#endif