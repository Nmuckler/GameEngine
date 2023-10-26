#include "GameManager.hpp"

float moveSpeed = 2;
bool hitboxActive = false;
Timeline timeline = Timeline();
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

    sf::RectangleShape *deathtangle = new sf::RectangleShape(sf::Vector2f(2500.f, 300.f));

    createPlatform(500, 2000, 0, 550, "Random");
    createPlatform(250, 30, 600, 550, "Random"); //moving platform 1 index line 226
    createPlatform(250, 30, 600, 200, "Random"); //moving platform 2 line 236
    createPlatform(250, 30, 0, 200, "Random");
    createPlatform(500, 2000, -800, 550, "Random");
    createPlatform(500, 2000, 1200, 550, "Random");




    DeathZone *deathzone1 = new DeathZone(deathtangle, sf::Vector2f(-1000, 675), "Transparent");

    deathObjects.push_back(deathzone1);
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

void GameManager::createPlatform(float xsize, float ysize, float xPos, float yPos, const std::string &texturePath)
{
    try
    {
        sf::RectangleShape *line = new sf::RectangleShape(sf::Vector2f(xsize, ysize));
        GameObject *platform = new GameObject(line, sf::Vector2f(xPos, yPos), texturePath);
        gameObjects.push_back(platform);
    }
    catch (...)
    {
        std::cerr << "Platform caught exception." << std::endl;
    }
}

void GameManager::createCharacter(int id)
{
    clientID = id;

    // printf("created a new character...\n");
    try
    {
        sf::CircleShape *newActorCircle = new sf::CircleShape(35.f);
        Actor *character = new Actor(newActorCircle, sf::Vector2f(100, 450), "hamster.png", ++numClients);
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
    actorMap[id]->getShape().setFillColor(sf::Color::Transparent);
}

bool GameManager::checkInputs(sf::RenderWindow *window)
{
    int idx = clientID;
    bool newInput = false;

    if (idx >= 0 && clientID <= actorMap.size())
    {

        bool checkGround = false;
        for (int i = 0; i < gameObjects.size(); i++)
        {
            if (actorMap[idx]->isTouching(gameObjects[i]->getShape()))
            {
                actorMap[idx]->velocityY = 0;
                grounded = true;
                checkGround = true;
            }
        }
        if(!checkGround){
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
            return false;
        }

        // Turn hit boxes on or off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            hitboxActive = !hitboxActive;
            newInput = true;
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
            newInput = true;
        }

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        {
            // printf("one pressed");
            timeline.changeTic(4);
            newInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        {
            // printf("two pressed");
            timeline.changeTic(2);
            newInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        {
            // printf("three pressed");
            timeline.changeTic(1);
            newInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
        {
            actorMap[idx]->setSpawn(actorMap[idx]->positionX, actorMap[idx]->positionY);
            newInput = true;
        }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            // Left key is pressed: move our character to the left
            actorMap[idx]->moveLeft(moveSpeed);
            newInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // Right key is pressed: move our character to the right
            actorMap[idx]->moveRight(moveSpeed);
            newInput = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            if (grounded)
            {
                actorMap[idx]->jump();
            }
            newInput = true;
        }
    }
    else
    {
        // Handle the out-of-bounds case (e.g., print an error message)
        std::cout << "Invalid actor ID: " << idx << std::endl;
    }
    return newInput;
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
                if (gameObjects.size() >= 2)
                    gameObjects[1]->getShape().setPosition(platformX, platformY);
                // std::cout << "Set position to: " << platformY << std::endl;
            }
        }
        else if (clientData.substr(0, 9) == "platform2")
        {
            float platformX, platformY;
            if (sscanf(clientData.c_str() + 9, "%f,%f", &platformX, &platformY) == 2)
            {
                if (gameObjects.size() >= 3)
                    gameObjects[2]->getShape().setPosition(platformX, platformY);
                // std::cout << "Set position to: " << platformY << std::endl;
            }
        }
        else if (clientData.substr(0, 7) == "deleted")
        {
            int deletedId;
            if (sscanf(clientData.c_str() + 7, "%d", &deletedId) == 1)
            {
                // std::cout << "Deleting client: " << deletedId << std::endl;
                if(deletedId == clientID)
                    abort();
                deleteCharacter(deletedId);
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

    for (auto it = actorMap.begin(); it != actorMap.end(); ++it)
    {
        stream += toString(it->first);
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

    if (!timeline.isPaused())
    {
        actorMap[clientID]->update(sentTime, grounded);
    }
    // Draw the character
    for (auto it = actorMap.begin(); it != actorMap.end(); ++it)
    {
        actorMap[it->first]->draw(window, hitboxActive);
    }
    for (int i = 0; i < gameObjects.size(); i++)
    {
        gameObjects[i]->draw(window, hitboxActive);
    }
    for (int i = 0; i < deathObjects.size(); i++)
    {
        deathObjects[i]->draw(window, hitboxActive);
    }


    // deathObjects[1]->draw(window, hitboxActive);
    // printf("leftBound: %f\n", viewLeft);
    // printf("rightBound: %f\n", viewRight);

    // updateView();
    window.setView(*gameview);
    window.display();
}