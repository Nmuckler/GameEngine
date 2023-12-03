#include "UserInputHandler.hpp"

void UserInputHandler::onEvent(Event event)
{
    float moveSpeed = 3;
    if (event.getEvent() == event.type::COLLISION || event.getEventInfo() == "USERINPUT")
    {
        // std::cout << "I was called" << std::endl;

        // Turn hit boxes on or off
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        // {
        //     hitboxActive = !hitboxActive;
        //     newInput = true;
        //     inputs[0] = true;
        // }
        if (event.booleanArray[1])
        {
            if (!event.timeline->isPaused())
            {
                // printf("paused pressed");
                event.timeline->pause();
            }
            else
            {
                event.timeline->unpause();
            }
        }

        if (event.booleanArray[2])
        {
            // printf("one pressed");
            event.timeline->changeTic(4);
        }
        if (event.booleanArray[3])
        {
            // printf("two pressed");
            event.timeline->changeTic(2);
        }
        if (event.booleanArray[4])
        {
            // printf("three pressed");
            event.timeline->changeTic(1);
        }
        if (event.booleanArray[5])
        {
            event.actor->setSpawn(event.actor->positionX, event.actor->positionY);
        }

        // Controls
        if (event.booleanArray[6])
        {
            // Left key is pressed: move our character to the left
            // std::cout << "Left key is pressed: move our character to the left" << std::endl;

            event.actor->moveLeft(moveSpeed);
        }
        if (event.booleanArray[7])
        {
            // Right key is pressed: move our character to the right
            // std::cout << "Right key is pressed: move our character to the right" << std::endl;

            event.actor->moveRight(moveSpeed);
        }
        if (event.booleanArray[8])
        {
            if (event.actor->isGrounded)
            {
                event.actor->jump();
            }
        }
    }
}