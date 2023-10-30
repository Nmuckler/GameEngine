#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "Actor.hpp"
#include "Object.hpp"
#include "DeathZone.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include "Bound.hpp"
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
    int clientID;
    GameManager();
    static GameManager *getInstance();
    void initialize(sf::View *gameview);
    void updateDeltaTime();
    void createPlatform(float xsize, float ysize, float xPos, float yPos, const std::string &texturePath);
    void createCharacter(int id);
    void deleteCharacter(int id);
    bool checkInputs(sf::RenderWindow *window); // check input for the specific Client runnning
    void parsePos(std::string str);
    void setBounds();
    void updateView();
    void render(sf::RenderWindow &window);
    std::string toString(int idx);
    std::string allToString();

    // Accessor method to get a specific game object by index
    // std::vector<Actor *> actorObjects;
    std::map<int, Actor*> actorMap;
    // std::iterator<int, Actor*> it;
    std::vector<GameObject *> gameObjects;
    std::vector<DeathZone *> deathObjects;
    std::vector<Bound *> bounds;


    sf::View *gameview;
    float viewLeft;
    float viewRight;

private:
    static GameManager *singleton;

    GameManager(const GameManager &obj) = delete;
};

#endif
