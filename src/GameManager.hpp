#ifndef GAMEMANAGER_HPP
#define GAMEMANAGER_HPP

#include "Actor.hpp"
#include "Object.hpp"
#include "GameObject.hpp"
#include "Timeline.hpp"
#include <vector>
#include <iostream>


class GameManager
{
public:
    GameManager();
    static GameManager *getInstance();
    void initialize();
    void updateDeltaTime();
    void createCharacter();
    void checkInputs(int id, char input);
    void render(sf::RenderWindow &window);

    // Accessor method to get a specific game object by index
    GameObject &getGameObjectAtIndex(int index);
    std::vector<Actor*> actorObjects;
    std::vector<GameObject*> gameObjects;

private:
    static GameManager *singleton;

    GameManager(const GameManager &obj) = delete;
};

#endif
