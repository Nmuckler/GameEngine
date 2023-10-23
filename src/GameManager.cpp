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

    sf::RectangleShape *line = new sf::RectangleShape(sf::Vector2f(500.f, 2000.f));
    sf::RectangleShape *movingLine = new sf::RectangleShape(sf::Vector2f(250.f, 30.f));

    GameObject *floor = new GameObject(line, sf::Vector2f(0, 550), "");
    GameObject *platform = new GameObject(movingLine, sf::Vector2f(600, 550), "");

    gameObjects.push_back(floor);
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

void GameManager::createCharacter()
{
    printf("created a new character...\n");
    try
    {
        sf::CircleShape *newActorCircle = new sf::CircleShape(35.f);
        Actor *character = new Actor(newActorCircle, sf::Vector2f(100, 450), "hamster.png", actorObjects.size() + 1);
        actorObjects.push_back(character);
        numClients++;
    }
    catch (...)
    {
        std::cerr << "Character caught exception." << std::endl;
    }
}

void GameManager:: deleteCharacter(int id)
{
}

void GameManager::checkInputs(sf::RenderWindow *window, int i)
{
    int id = i - 1;
    // printf("The size of actor objects is: %ld\n", actorObjects.size());
    // for (int i = 0; i < actorObjects.size(); i++)
    // {
    // printf("Checking inputs for actor %d", i);

    if (!window->hasFocus())
        return;

    if (id >= 0 && id < actorObjects.size())
    {

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
            printf("one pressed");
            timeline.changeTic(4);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            printf("two pressed");
            timeline.changeTic(2);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            printf("three pressed");
            timeline.changeTic(1);
        }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Left key is pressed: move our character to the left
            actorObjects[id]->moveLeft(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Right key is pressed: move our character to the right
            actorObjects[id]->moveRight(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (grounded)
            {
                actorObjects[id]->jump();
            }
        }
    }
    else
    {
        // Handle the out-of-bounds case (e.g., print an error message)
        std::cout << "Invalid actor ID: " << id << std::endl;
    }
}

void GameManager::parsePos(std::string str, int clientID)
{
    std::istringstream iss(str);
    std::string clientData;

    while (std::getline(iss, clientData, '|'))
    {
        // Check if this is client data or platform data
        if (clientData.substr(0, 8) == "platform")
        {
            float platformX, platformY;
            if (sscanf(clientData.c_str() + 8, "%f,%f", &platformX, &platformY) == 2)
            {
                gameObjects[1]->getShape().setPosition(platformX, platformY);
                // std::cout << "Set position to: " << platformY << std::endl;
            }
        }
        else
        {
            int id;
            float xpos, ypos;

            if (sscanf(clientData.c_str(), "%d,%f,%f", &id, &xpos, &ypos) == 3)
            {
                if (id != clientID)
                {
                    while (id > actorObjects.size())
                    {
                        createCharacter();
                    }
                    actorObjects[id - 1]->getShape().setPosition(xpos, ypos);
                }
            }
        }
    }
}
// void GameManager::parsePos(std::string str, int clientID)
// {
//     std::istringstream iss(str);
//     std::string clientData;

//     while (std::getline(iss, clientData, '|'))
//     {
//         int id;
//         float xpos, ypos;

//         if (sscanf(clientData.c_str(), "%d,%f,%f", &id, &xpos, &ypos) == 3)
//         {
//             if (id != clientID)
//             {
//                 while (id > actorObjects.size())
//                 {
//                     createCharacter();
//                 }
//                 actorObjects[id - 1]->getShape().setPosition(xpos, ypos);
//             }
//         }
//     }
// }

std::string GameManager::toString(int id)
{
    int idx = id - 1;
    std::string stream = "";
    stream += std::to_string(actorObjects[idx]->getId()) + "," + std::to_string(actorObjects[idx]->positionX) + "," + std::to_string(actorObjects[idx]->positionY) + "|";

    return stream;
}

std::string GameManager::allToString()
{
    std::string stream = ""; // Initialize the stream

    for (int i = 1; i <= actorObjects.size(); i++)
    {
        stream += toString(i);
    }

    return stream;
}

void GameManager::render(sf::RenderWindow &window, int clientID)
{
    window.clear(sf ::Color::White);

    // gameObjects[1]->hoverY(200, 550, .5);

    if (!timeline.isPaused())
    {
        actorObjects[clientID - 1]->update(sentTime, grounded);

        // if (clientID == 1) //other clients get platform info from another client
        // {
        //     for (int i = 0; i < gameObjects.size(); i++)
        //     {
        //         gameObjects[i]->update(sentTime);
        //     }
        // }
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