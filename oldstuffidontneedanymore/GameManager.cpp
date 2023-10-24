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

sf::CircleShape newActorCircle(35.f);

sf::RectangleShape line(sf::Vector2f(400.f, 500.f));
sf::RectangleShape movingLine(sf::Vector2f(250.f, 30.f));

Actor character = Actor(newActorCircle, sf::Vector2f(100, 500), "hamster.png");
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

    actorObjects.push_back(character);
    gameObjects.push_back(_floor);
    gameObjects.push_back(platform);
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

void GameManager::checkInputs()
{
    for (int i = 0; i < actorObjects.size(); i++)
    {

        // compute grounded
        if (actorObjects[i].isTouchingFloor(gameObjects[0].getShape()) || actorObjects[i].isTouchingFloor(gameObjects[1].getShape()))
        {
            grounded = true;
        }
        else
        {
            grounded = false;
        }
        // Gravity
        if (actorObjects[i].isTouchingFloor(gameObjects[0].getShape()) || actorObjects[i].isTouchingFloor(gameObjects[1].getShape()))
        {
            actorObjects[i].velocityY = 0;
        }

        // Turn hit boxes on or off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            hitboxActive = !hitboxActive;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            // printf("one pressed");
            timeline.changeTic(4);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            // printf("two pressed");
            timeline.changeTic(2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            // printf("three pressed");
            timeline.changeTic(1);
        }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Left key is pressed: move our character to the left
            actorObjects[i].moveLeft(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Right key is pressed: move our character to the right
            actorObjects[i].moveRight(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (grounded)
            {
                actorObjects[i].jump();
            }
        }
    }
}

void GameManager::render(sf::RenderWindow &window)
{
    window.clear(sf ::Color::White);

    gameObjects[1].hoverY(200, 550, .5);
    gameObjects[0].hoverX(200, 550, .5);

    if (!timeline.isPaused())
    {
        printf("sentTime: %lf\n", sentTime);
        for (int i = 0; i < actorObjects.size(); i++)
        {
            actorObjects[i].update(sentTime, grounded);
        }
        for (int i = 0; i < gameObjects.size(); i++)
        {
            gameObjects[i].update(sentTime);
        }
    }
    // Draw the character
    for (int i = 0; i < actorObjects.size(); i++)
    {
        actorObjects[i].draw(window, hitboxActive);
    }
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i].draw(window, hitboxActive);
    }

    // Display the updated frame
    window.display();
}