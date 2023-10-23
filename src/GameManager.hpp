#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "Actor.hpp"
#include "Object.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include <vector>
#include <iostream>
#include <SFML/Graphics.hpp>
#include <cstdio>
#include <cstdlib>
#include <string>
#include <sstream>
#include <SFML/Window.hpp>
#include <SFML/OpenGL.hpp>

class GameManager
{
public:
    int numClients = 0;
    GameManager();
    static GameManager *getInstance();
    void initialize();
    void updateDeltaTime();
    void createCharacter();
    void deleteCharacter(int id);
    void checkInputs(sf::RenderWindow *window, int id); //check input for the specific Client runnning
    void parsePos(std::string str, int clientID);
    void render(sf::RenderWindow &window, int clientID);
    std::string toString(int idx);
    std::string allToString();

    // Accessor method to get a specific game object by index
    std::vector<Actor*> actorObjects;
    std::vector<GameObject*> gameObjects;

private:
    static GameManager *singleton;

    GameManager(const GameManager &obj) = delete;
};

#endif
