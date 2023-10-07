#include "GameObject.hpp"

GameObject::GameObject(sf::Shape& shape, sf::Vector2f position, const std::string& texturePath):
Object(shape, position, texturePath) {
    this->moveX = true;
    this->moveY = true;
}


void GameObject::hoverX(int x1, int x2, float speed) {
    //make sure x1 is lower value
    int newx1 = x1;
    int newx2 = x2;
    float newSpeed;
    if(x1 > x2){
        newx1 = x2;
        newx2 = x1;
    }
    if(speed < 0){
        newSpeed = speed *= -1; //speed was negative
    } else {
        newSpeed = speed;
    }

    //true if moving left
    if(moveX){
        if(positionX <= newx1){ //position is lower than the lower bound
            moveX = !moveX;
            //this->shape.move(newSpeed, 0.f);
        } else {
            moveLeft(newSpeed);
        }
    } else {
        if(positionX >= newx2){ //position is higher than the higher bound
            moveX = !moveX;
            //this->shape.move((newSpeed * -1), 0.f);
        } else {
            moveRight(newSpeed);
        }
    }
}
void GameObject::hoverY(int y1, int y2, float speed) {
    //make sure x1 is lower value
    int newy1 = y1;
    int newy2 = y2;
    float newSpeed;
    if(y1 > y2){
        newy1 = y2;
        newy2 = y1;
    }
    if(speed < 0){
        newSpeed = speed *= -1; //speed was negative
    } else {
        newSpeed = speed;
    }

    //true if moving left
    if(moveY){
        if(positionY <= newy1){ //position is lower than the lower bound
            moveY = !moveY;
            //this->shape.move(0.f, newSpeed);
        } else { //y pos is greater than newy1, therefore move less
            moveUp(newSpeed);
        }
    } else {
        if(positionY >= newy2){ //position is higher than the higher bound
            moveY = !moveY;
            //this->shape.move(0.f, (newSpeed * -1));
        } else { //y pos is greater than newy1, therefore move less
            moveDown(newSpeed);
        }
    }
}