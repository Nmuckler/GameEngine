#include "GameManager.hpp"

float moveSpeed = 2;
bool hitboxActive = false;
Timeline timeline = Timeline();
// Timeline characterTimeline = Timeline(&timeline, 1); //WILL NEED TO CREATE ARRAY OF TIMELINES
int64_t currTime = 0;
int64_t lastTime = 0;
int64_t deltaTime = 0;
bool grounded = true;
double sentTime = 0;

sf::CircleShape character1Circle(35.f);
sf::CircleShape character2Circle(35.f);

sf::RectangleShape line(sf::Vector2f(400.f, 500.f));
sf::RectangleShape movingLine(sf::Vector2f(250.f, 30.f));

// Actor character1 = Actor(character1Circle, sf::Vector2f(100, 450), "hamster.png");
// Actor character2 = Actor(character2Circle, sf::Vector2f(250, 450), "hamster.png");

GameObject _floor = GameObject(line, sf::Vector2f(0, 550), "");
GameObject platform = GameObject(movingLine, sf::Vector2f(600, 550), "");

GameManager *GameManager::singleton = nullptr;
/**
 * Used https://www.geeksforgeeks.org/implementation-of-singleton-class-in-cpp/ for help
 * and also used 216 project 1 for help
 */
GameManager::GameManager()
{
}

GameManager *GameManager::getInstance()
{
    if (singleton == nullptr)
    {
        singleton = new GameManager();
    }
    return singleton;
}

void GameManager::initialize()
{

    // sf::CircleShape newActorCircle(35.f);

    // sf::RectangleShape line(sf::Vector2f(800.f, 500.f));
    // sf::RectangleShape movingLine(sf::Vector2f(250.f, 30.f));

    // Actor character = Actor(newActorCircle, sf::Vector2f(100, 500), "hamster.png");
    // GameObject floor = GameObject(line, sf::Vector2f(0, 550), "");
    // GameObject platform = GameObject(movingLine, sf::Vector2f(600, 550), "");

    gameObjects.push_back(&_floor);
    gameObjects.push_back(&platform);
}

void GameManager::updateDeltaTime()
{
    currTime = timeline.getTime();
    deltaTime = currTime - lastTime;
    // printf("Currtime: %ld\n", currTime);
    // printf("last time: %ld\n", lastTime);
    // printf("deltatime: %ld\n", deltaTime);

    lastTime = currTime;
    sentTime = deltaTime;
}

void GameManager::createCharacter()
{
    printf("created a new character...\n");
    try
    {
        sf::CircleShape *newActorCircle =  new sf::CircleShape(35.f);
        Actor *character = new Actor(newActorCircle, sf::Vector2f(100, 450), "hamster.png");
        actorObjects.push_back(character);
        // if (actorObjects.size() == 1)
        // {
        //     actorObjects.push_back(&character1);
        // } else {
        //     actorObjects.push_back(&character1);
        // }
    }
    catch (...)
    {
        std::cerr << "Character caught exception." << std::endl;
    }
    // sf::CircleShape newActorCircle(35.f);
    // Actor character = Actor(newActorCircle, sf::Vector2f(100, 500), "hamster.png");
    // actorObjects.push_back(&character);
}

void GameManager::checkInputs(int i, char input)
{
    int id = i - 1;
    // printf("The size of actor objects is: %ld\n", actorObjects.size());
    for (int i = 0; i < actorObjects.size(); i++)
    {
        // printf("Checking inputs for actor %d", i);

        // compute grounded
        if (actorObjects[id]->isTouchingFloor(gameObjects[0]->getShape()) || actorObjects[id]->isTouchingFloor(gameObjects[1]->getShape()))
        {
            // printf("Setting to true\n");
            grounded = true;
            actorObjects[id]->velocityY = 0;
        }
        else
        {
            grounded = false;
        }
        // Gravity
        // if (actorObjects[id].isTouchingFloor(gameObjects[0].getShape()) || actorObjects[id].isTouchingFloor(gameObjects[1].getShape()))
        // {
        //     actorObjects[id].velocityY = 0;
        // }

        // Turn hit boxes on or off
        if (input == 'H')
        {
            hitboxActive = !hitboxActive;
        }
        if (input == 'P')
        {
            if (!timeline.isPaused())
            {
                printf("paused pressed");
                timeline.pause();
            }
            else
            {
                timeline.unpause();
            }
        }
        if (input == '1')
        {
            printf("one pressed");
            timeline.changeTic(4);
        }
        if (input == '2')
        {
            printf("two pressed");
            timeline.changeTic(2);
        }
        if (input == '3')
        {
            printf("three pressed");
            timeline.changeTic(1);
        }

        // Controls
        if (input == 'L')
        {
            // Left key is pressed: move our character to the left
            actorObjects[id]->moveLeft(moveSpeed);
        }
        if (input == 'R')
        {
            // Right key is pressed: move our character to the right
            actorObjects[id]->moveRight(moveSpeed);
        }
        if (input == ' ')
        {
            if (grounded)
            {
                actorObjects[id]->jump();
            }
        }
    }
}

void GameManager::render(sf::RenderWindow &window)
{
    window.clear(sf ::Color::White);

    gameObjects[1]->hoverY(200, 550, .5);
    // gameObjects[0].hoverX(200, 550, .5);

    if (!timeline.isPaused())
    {
        // printf("sentTime: %lf\n", sentTime);
        for (int i = 0; i < actorObjects.size(); i++)
        {
            actorObjects[i]->update(sentTime, grounded);
        }
        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i]->update(sentTime);
        }
    }
    // Draw the character
    for (int i = 0; i < actorObjects.size(); i++)
    {
        actorObjects[i]->draw(window, hitboxActive);
    }
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->draw(window, hitboxActive);
    }

    // Display the updated frame
    window.display();
}