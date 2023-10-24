#include "Object.hpp"

Object::Object(sf::Shape &shape, sf::Vector2f position, const std::string &texturePath) : shape(shape), texture(texture)
{
    if (texturePath == "")
    {
        this->shape.setFillColor(sf::Color::Red);
    }
    else if (colorMap.find(texturePath) != colorMap.end())
    {
        this->shape.setFillColor(colorMap.at(texturePath));
    }
    else if (!texture.loadFromFile(texturePath))
    {
        this->shape.setFillColor(sf::Color::Magenta);
    }
    else
    {
        this->shape.setTexture(&texture);
    }
    this->shape.setPosition(position);
    positionX = position.x;
    positionY = position.y;
    velocityX = 0;
    velocityY = 0;
}

void Object::draw(sf::RenderWindow &window, bool drawBoundingBox)
{
    if (drawBoundingBox)
    { // shows hit boxes
        sf::FloatRect bounds = this->shape.getGlobalBounds();
        sf::RectangleShape boundingBox(sf::Vector2f(bounds.width, bounds.height));
        boundingBox.setPosition(bounds.left, bounds.top);
        boundingBox.setOutlineColor(sf::Color::Green);
        boundingBox.setOutlineThickness(1.0f);
        boundingBox.setFillColor(sf::Color::Transparent);
        window.draw(boundingBox);
    }
    window.draw(shape);
}

sf::Shape &Object::getShape()
{
    return shape;
}

void Object::move(float velX, float velY)
{
    // this->shape.setPosition(positionX + velX, positionY + velY);
    velocityX = velX;
    velocityY = velY;
}

void Object::moveLeft(float velocity)
{
    velocityX = velocity * -1;
}
void Object::moveRight(float velocity)
{
    velocityX = velocity;
}

void Object::moveUp(float velocity)
{
    velocityY = velocity * -1;
}
void Object::moveDown(float velocity)
{
    velocityY = velocity;
}

void Object::jump()
{
    velocityY = -4;
}

bool Object::isTouching(const sf::Shape &object)
{
    sf::FloatRect actorBounds = this->shape.getGlobalBounds();
    sf::FloatRect otherBounds = object.getGlobalBounds();

    if (actorBounds.intersects(otherBounds))
    {
        // velocityY = 0;
        // printf("I am colliding");
        return true;
    }
    return false;
}

void Object::update(int64_t deltaTime)
{
    double time = deltaTime;
    time /= 10;

    float newPosY = positionY + (velocityY * time);
    if (velocityX < 1000 && velocityY < 1000 && newPosY < 10000 && time < 1000 && time >= 0)
    {
        getShape().setPosition(positionX + (velocityX * time), newPosY);
        positionX = positionX + (velocityX * time);
        positionY = positionY + (velocityY * time);
    }
}

void Object::update(int64_t deltaTime, bool grounded)
{

    double time = deltaTime;
    // printf("Received time: %f\n", time);
    time /= 10; // making it smaller to make it more digestable in the enviroment
    // printf("Velocity x: %f\n", velocityX * time);
    // printf("Velocity y: %f\n", velocityY * time);
    // printf("Position x: %f\n", positionX);
    // printf("Position y: %f\n", positionY);

    float newPosY = positionY + (velocityY * time);
    float Ydelta = newPosY - positionY;
    if (velocityX < 20 && velocityY < 15 && std::abs(Ydelta) < 100 && time < 100 && time >= 0)
    {
        getShape().setPosition(positionX + (velocityX * time), newPosY);
        positionX = positionX + (velocityX * time);
        positionY = positionY + (velocityY * time);
    }
    if (!grounded && time < 1000 && time >= 0)
    {
        velocityY += gravity * time;
    }

    if (velocityX != 0)
    {
        velocityX *= .5;
    }
}
