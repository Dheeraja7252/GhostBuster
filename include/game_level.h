#ifndef GAMELEVEL_H
#define GAMELEVEL_H
#include <vector>

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "game_object.h"
#include "sprite_renderer.h"
#include "resource_manager.h"

class GameLevel
{
public:
    // level state
    std::vector<GameObject> Walls;
    std::vector<GameObject> Enemies;
    std::vector<GameObject> Coins;
    GameObject Player;

    // constructor
    GameLevel() { }
    // render level
    void Draw(SpriteRenderer &renderer);
    // check if the level is completed
    bool IsCompleted();
    // initialize level from tile data
    void init(int numWalls, int numEnemies, int numCoins, int X, int Y);
};

#endif

