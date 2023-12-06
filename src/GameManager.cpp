#include "GameManager.hpp"

float moveSpeed = 4;
bool hitboxActive = false;
int64_t currTime = 0;
int64_t lastTime = 0;
int64_t deltaTime = 0;
double sentTime = 0;

sf::RectangleShape *newActorRectangle = new sf::RectangleShape(sf::Vector2f(20.f, 125));
Actor *player2 = new Actor(newActorRectangle, sf::Vector2f(700, 450), "Random", 2);

sf::CircleShape *newActorCircle = new sf::CircleShape(10.f);
GameObject *ball = new GameObject(newActorCircle, sf::Vector2f(400, 300), "Random");

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
    timeline = Timeline();
    gameview = view;

    sf::RectangleShape *deathtangle = new sf::RectangleShape(sf::Vector2f(25000.f, 300.f));
    sf::RectangleShape *deathtangle2 = new sf::RectangleShape(sf::Vector2f(300.f, 3000.f));

    createPlatform(2000, 2000, -400, 599, "Red");
    // createPlatform(250, 30, 600, 550, "Blue");   // moving platform 1 index line 226
    // createPlatform(250, 30, 600, 200, "Yellow"); // moving platform 2 line 236
    // createPlatform(500, 2000, -800, 550, "Cyan");
    createPlatform(2000, 2000, -400, -1999, "Magenta");

    // DeathZone *deathzone1 = new DeathZone(deathtangle, sf::Vector2f(-10000, 675), "Transparent");
    // DeathZone *deathzone2 = new DeathZone(deathtangle2, sf::Vector2f(-300, 200), "White");

    // deathObjects.push_back(deathzone1);
    // deathObjects.push_back(deathzone2);
    actorMap[2] = player2;
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

        sf::RectangleShape *newActorRectangle = new sf::RectangleShape(sf::Vector2f(20.f, 125));
        Actor *character = character = new Actor(newActorRectangle, sf::Vector2f(75, 450), "Random", ++numClients);
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

void GameManager::checkCollisions()
{
    bool checkGround = false;
    for (int i = 0; i < gameObjects.size(); i++)
    {
        if (actorMap[clientID]->isTouching(gameObjects[i]->getShape()))
        {
            // actorMap[clientID]->velocityY = 0;
            // actorMap[clientID]->isGrounded = true;
            Event *collision = new Event(Event::COLLISION, "COLLISION", timeline.getTime() + 0, actorMap[clientID]);
            eventManager->raise(collision);
            checkGround = true;
        }
    }
    if (!checkGround)
    {
        actorMap[clientID]->isGrounded = false;
    }
}

void GameManager::checkDeath()
{

    // check for death
    if (actorMap[clientID]->alive)
    {
        for (int i = 0; i < deathObjects.size(); i++)
        {
            if (actorMap[clientID]->isTouching(deathObjects[i]->getShape()))
            {
                Event *death = new Event(Event::DEATH, "DEATH", timeline.getTime() + 0, actorMap[clientID]);
                eventManager->raise(death);
                Event *spawn = new Event(Event::SPAWN, "SPAWN", timeline.getTime() + 1000, actorMap[clientID]);
                eventManager->raise(spawn);
                break;
            }
        }
    }
}

bool GameManager::checkInputs(sf::RenderWindow *window)
{
    int idx = clientID;
    bool newInput = false;
    bool inputs[15] = {0};

    if (idx >= 0 && clientID <= actorMap.size() && actorMap[idx]->alive)
    {

        if (!window->hasFocus())
        {
            return false;
        }

        // Turn hit boxes on or off
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::H))
        {
            hitboxActive = !hitboxActive;
            newInput = true;
            inputs[0] = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::P))
        {
            // if (!timeline.isPaused())
            // {
            //     // printf("paused pressed");
            //     timeline.pause();
            // }
            // else
            // {
            //     timeline.unpause();
            // }
            newInput = true;
            inputs[1] = true;
        }

        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num1))
        // {
        //     // printf("one pressed");
        //     // timeline.changeTic(4);
        //     newInput = true;
        //     inputs[2] = true;
        // }
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num2))
        // {
        //     // printf("two pressed");
        //     // timeline.changeTic(2);
        //     newInput = true;
        //     inputs[3] = true;
        // }
        // // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num3))
        // // {
        // //     // printf("three pressed");
        // //     // timeline.changeTic(1);
        // //     newInput = true;
        // //     inputs[4] = true;
        // // }
        // if (sf::Keyboard::isKeyPressed(sf::Keyboard::Num8))
        // {
        //     // actorMap[idx]->setSpawn(actorMap[idx]->positionX, actorMap[idx]->positionY);
        //     newInput = true;
        //     inputs[5] = true;
        // }

        // Controls
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
        {
            // Left key is pressed: move our character to the left
            // actorMap[idx]->moveLeft(moveSpeed);
            newInput = true;
            inputs[6] = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
        {
            // Right key is pressed: move our character to the right
            // actorMap[idx]->moveRight(moveSpeed);
            newInput = true;
            inputs[7] = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space))
        {
            // if (actorMap[idx]->isGrounded)
            // {
            //     actorMap[idx]->jump();
            // }
            newInput = true;
            inputs[8] = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
        {
            // Left key is pressed: move our character to the left
            if (actorMap[2]->getShape().getPosition().y > 0)
                actorMap[2]->getShape().move(0, -1 * moveSpeed);
            // printf("Up\n");
            // newInput = true;
            // inputs[9] = true;
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
        {
            // Right key is pressed: move our character to the right
            if (actorMap[2]->getShape().getPosition().y < 450)
                actorMap[2]->getShape().move(0, moveSpeed);
            // printf("Down\n");
            // newInput = true;
            // inputs[10] = true;
        }
    }
    else
    {
        // Handle the out-of-bounds case (e.g., print an error message)
        // std::cout << "Invalid actor ID: " << idx << std::endl;
    }
    if (newInput)
    {
        // std::cout << "Sending new input" << std::endl;
        // std::cout << "booleanArray: ";
        // for (int i = 0; i < 15; ++i)
        // {
        //     std::cout << inputs[i] << " ";
        // }
        // std::cout << std::endl;
        Event *userinput = new Event(Event::USERINPUT, "USERINPUT", timeline.getTime() + 0, actorMap[clientID], inputs, &timeline);
        eventManager->raise(userinput);
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

        if (clientData.substr(0, 7) == "deleted")
        {
            int deletedId;
            if (sscanf(clientData.c_str() + 7, "%d", &deletedId) == 1)
            {
                // std::cout << "Deleting client: " << deletedId << std::endl;
                if (deletedId == clientID)
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

void GameManager::parseEnv(std::string str)
{
    // std::istringstream iss(str);
    // std::string clientData;

    // while (std::getline(iss, clientData, '|'))
    // {
    //     // Check if this is client data or platform data
    //     if (clientData.substr(0, 9) == "platform1")
    //     {
    //         float platformX, platformY;
    //         if (sscanf(clientData.c_str() + 9, "%f,%f", &platformX, &platformY) == 2)
    //         {
    //             if (gameObjects.size() >= 2)
    //                 gameObjects[1]->getShape().setPosition(platformX, platformY);
    //             // std::cout << "Set position to: " << platformY << std::endl;
    //         }
    //     }
    //     else if (clientData.substr(0, 9) == "platform2")
    //     {
    //         float platformX, platformY;
    //         if (sscanf(clientData.c_str() + 9, "%f,%f", &platformX, &platformY) == 2)
    //         {
    //             if (gameObjects.size() >= 3)
    //                 gameObjects[2]->getShape().setPosition(platformX, platformY);
    //             // std::cout << "Set position to: " << platformY << std::endl;
    //         }
    //     }
    // }
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
    while (bounds.size() != 0)
    {
        bounds.pop_back();
    }
    sf::RectangleShape *left = new sf::RectangleShape(sf::Vector2f(30.f, 1000.f));
    sf::RectangleShape *right = new sf::RectangleShape(sf::Vector2f(30.f, 1000.f));

    Bound *leftB = new Bound(left, sf::Vector2f(actorMap[1]->positionX - 100, -200), "Transparent");
    Bound *rightB = new Bound(right, sf::Vector2f(actorMap[2]->positionX + 100, -200), "Transparent");

    bounds.push_back(leftB);
    bounds.push_back(rightB);
    if (actorMap.size() > 0)
    {
        int xPos = actorMap[clientID]->positionX;
        int yPos = actorMap[clientID]->positionY;
        gameview->setCenter(xPos, yPos - 150);
    }
}
void GameManager::updateView()
{
    // double time = sentTime / 10;
    // if (time < 100 && time >= 0)
    // {
    //     float xVel = time * actorMap[clientID]->velocityX;

    //     if (actorMap[clientID]->isTouching(bounds[0]->getShape()) && xVel < 0)
    //     {
    //         gameview->move(xVel, 0);
    //         bounds[1]->getShape().move(xVel, 0);
    //         bounds[0]->getShape().move(xVel, 0);
    //     }
    //     else if (actorMap[clientID]->isTouching(bounds[1]->getShape()) && xVel > 0)
    //     {
    //         gameview->move(xVel, 0);
    //         bounds[1]->getShape().move(xVel, 0);
    //         bounds[0]->getShape().move(xVel, 0);
    //     }
    // }
}

void GameManager::updateBall()
{
    float xpos = ball->getShape().getPosition().x;
    if(xpos < 0 || xpos > 800){
        ball->getShape().setPosition(400, 400);
         ball->velocityX *= -1;
    }
    if (ball->velocityY > 0)
    {
        if (ball->isTouching(gameObjects[0]->getShape()))
        {
            ball->velocityY *= -1;
        }
    }
    else if (ball->velocityY < 0)
    {
        if (ball->isTouching(gameObjects[1]->getShape()))
        {
            ball->velocityY *= -1;
        }
    }


    if (ball->velocityX > 0)
    {
        if (ball->isTouching(actorMap[2]->getShape()))
        {
            ball->velocityX *= -1;
        }
    }
    else if (ball->velocityX < 0)
    {
        if (ball->isTouching(actorMap[1]->getShape()))
        {
            ball->velocityX *= -1;
        }
    }
    else
    {
        ball->velocityX = 1;
        ball->velocityY = 1;
    }
    ball->getShape().move(ball->velocityX * moveSpeed, ball->velocityY * moveSpeed);
}

void GameManager::render(sf::RenderWindow &window)
{
    // updateView();
    window.clear(sf ::Color::Black);

    if (!timeline.isPaused() && actorMap[clientID]->alive)
    {
        actorMap[clientID]->update(sentTime, actorMap[clientID]->isGrounded);
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
    updateBall();
    ball->draw(window, hitboxActive); // added for game
    for (int i = 0; i < deathObjects.size(); i++)
    {
        deathObjects[i]->draw(window, hitboxActive);
    }
    for (int i = 0; i < bounds.size(); i++)
    {
        bounds[i]->draw(window, hitboxActive);
    }

    // updateView();
    // window.setView(*gameview);
    window.display();
}