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

/**
 * This creates an instantiates the enviroment
 */
void GameManager::initialize(sf::View *view)
{
    gameview = view;

    sf::RectangleShape *line = new sf::RectangleShape(sf::Vector2f(500.f, 2000.f));
    sf::RectangleShape *movingLine = new sf::RectangleShape(sf::Vector2f(250.f, 30.f));
    sf::RectangleShape *movingLine2 = new sf::RectangleShape(sf::Vector2f(250.f, 30.f));

    sf::RectangleShape *deathtangle = new sf::RectangleShape(sf::Vector2f(2500.f, 300.f));

    sf::RectangleShape *leftTangle = new sf::RectangleShape(sf::Vector2f(10.f, 1000.f));
    sf::RectangleShape *rightTangle = new sf::RectangleShape(sf::Vector2f(10.f, 1000.f));

    // create platforms
    GameObject *floor = new GameObject(line, sf::Vector2f(0, 550), "Random");
    GameObject *platform = new GameObject(movingLine, sf::Vector2f(600, 550), "Blue");
    GameObject *platform2 = new GameObject(movingLine2, sf::Vector2f(600, 200), "Magenta");

    DeathZone *deathzone1 = new DeathZone(deathtangle, sf::Vector2f(-1000, 625), "Transparent");

    // Bound *leftBound = new Bound(leftTangle, sf::Vector2f(-100, 0), "Red");
    // Bound *rightBound = new Bound(rightTangle, sf::Vector2f(300, 0), "Red");

    gameObjects.push_back(floor);
    gameObjects.push_back(platform);
    gameObjects.push_back(platform2);
    deathObjects.push_back(deathzone1);

    // bounds.push_back(leftBound);
    // bounds.push_back(rightBound);
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

void GameManager::createCharacter(int id)
{
    clientID = id;

    // printf("created a new character...\n");
    try
    {
        sf::CircleShape *newActorCircle = new sf::CircleShape(35.f);
        Actor *character = new Actor(newActorCircle, sf::Vector2f(100, 450), "hamster.png", ++numClients);
        // actorObjects.push_back(character);
        // numClients++;
        actorMap[numClients] = character;
        std::cerr << "Created client:" << numClients << std::endl;


    }
    catch (...)
    {
        std::cerr << "Character caught exception." << std::endl;
    }
}

void GameManager::deleteCharacter(int id)
{
}

void GameManager::checkInputs(sf::RenderWindow *window)
{
    int idx = clientID;


    if (idx >= 0 && clientID <= actorMap.size())
    {

        // compute grounded
        if (actorMap[idx]->isTouching(gameObjects[0]->getShape()) || actorMap[idx]->isTouching(gameObjects[1]->getShape()) || actorMap[idx]->isTouching(gameObjects[2]->getShape()))
        {
            // printf("Setting to true\n");
            actorMap[idx]->velocityY = 0;
            grounded = true;
        }
        else
        {
            grounded = false;
        }

        // check for death
        if (actorMap[idx]->isTouching(deathObjects[0]->getShape()))
        {
            actorMap[idx]->respawn();
            setBounds();
        }

        if (!window->hasFocus())
        {
            return;
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
                // printf("paused pressed");
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
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
        {
            actorMap[idx]->setSpawn(actorMap[idx]->positionX, actorMap[idx]->positionY);
        }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Left key is pressed: move our character to the left
            actorMap[idx]->moveLeft(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Right key is pressed: move our character to the right
            actorMap[idx]->moveRight(moveSpeed);
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (grounded)
            {
                actorMap[idx]->jump();
            }
        }
    }
    else
    {
        // Handle the out-of-bounds case (e.g., print an error message)
        std::cout << "Invalid actor ID: " << idx << std::endl;
    }
}

void GameManager::parsePos(std::string str)
{
    std::istringstream iss(str);
    std::string clientData;

    while (std::getline(iss, clientData, '|'))
    {
        // Check if this is client data or platform data
        if (clientData.substr(0, 9) == "platform1")
        {
            float platformX, platformY;
            if (sscanf(clientData.c_str() + 9, "%f,%f", &platformX, &platformY) == 2)
            {
                gameObjects[1]->getShape().setPosition(platformX, platformY);
                // std::cout << "Set position to: " << platformY << std::endl;
            }
        }
        else if (clientData.substr(0, 9) == "platform2")
        {
            float platformX, platformY;
            if (sscanf(clientData.c_str() + 9, "%f,%f", &platformX, &platformY) == 2)
            {
                gameObjects[2]->getShape().setPosition(platformX, platformY);
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
                    while (id > actorMap.size())
                    {
                        createCharacter(clientID);
                    }
                    actorMap[id]->getShape().setPosition(xpos, ypos);
                }
            }
        }
    }
}

std::string GameManager::toString(int id)
{
    int idx = id;
    std::string stream = "";
    stream += std::to_string(actorMap[idx]->getId()) + "," + std::to_string(actorMap[idx]->positionX) + "," + std::to_string(actorMap[idx]->positionY) + "|";

    return stream;
}

std::string GameManager::allToString()
{
    std::string stream = ""; // Initialize the stream

    for (int i = 1; i <= actorMap.size(); i++)
    {
        stream += toString(i);
    }

    return stream;
}
void GameManager::setBounds()
{
    if (actorMap.size() > 0)
    {
        int xPos = actorMap[clientID]->positionX;
        int yPos = actorMap[clientID]->positionY;

        viewLeft = xPos - 100;
        viewRight = xPos + 100;
        gameview->setCenter(xPos, yPos - 150);
    }
    else
    {
        printf("NO ACTORS HERE\n");
    }
}
void GameManager::updateView()
{
    double time = sentTime / 10;
    if (time < 100 && time >= 0)
    {
        float xPos = actorMap[clientID]->positionX;
        float xVel = time * actorMap[clientID]->velocityX;

        if (xPos <= viewLeft && xVel < 0)
        {
            gameview->move(xVel, 0);
            viewRight += xVel;
            viewLeft += xVel;
        }
        else if (xPos >= viewRight && xVel > 0)
        {
            gameview->move(xVel, 0);
            viewRight += xVel;
            viewLeft += xVel;
        }
    }
}

void GameManager::render(sf::RenderWindow &window)
{
    updateView();
    window.clear(sf ::Color::Black);

    // gameObjects[1]->hoverY(200, 550, .5);

    if (!timeline.isPaused())
    {
        actorMap[clientID]->update(sentTime, grounded);

        // if (clientID == 1) //other clients get platform info from another client
        // {
        //     for (int i = 0; i < gameObjects.size(); i++)
        //     {
        //         gameObjects[i]->update(sentTime);
        //     }
        // }
    }
    // Draw the character
    for (int i = 1; i <= actorMap.size(); i++)
    {
        actorMap[i]->draw(window, hitboxActive);
    }
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->draw(window, hitboxActive);
    }
    for (int i = 0; i < deathObjects.size(); i++)
    {
        deathObjects[i]->draw(window, hitboxActive);
    }

    // bounds[0]->draw(window, hitboxActive);
    // bounds[1]->draw(window, hitboxActive);

    // deathObjects[1]->draw(window, hitboxActive);
    // printf("leftBound: %f\n", viewLeft);
    // printf("rightBound: %f\n", viewRight);

    // updateView();
    window.setView(*gameview);
    window.display();
}