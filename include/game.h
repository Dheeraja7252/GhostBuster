#ifndef GAME_H
#define GAME_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "game_level.h"

enum GameState {
    GAME_ACTIVE,
    GAME_MENU,
    GAME_WIN,
    GAME_LOSE,
    LEVEL_WIN
};


class Game
{
public:
    GameState               State;
    bool                    Keys[1024];
    unsigned int            Width, Height;
    std::vector<GameLevel> Levels;
    unsigned int           Level;
    float PLAYER_VELOCITY;
    float ENEMY_VELOCITY;
    int score;
    float TotalTimePlayed;

    Game(unsigned int width, unsigned int height);
    ~Game();

    void Init();
    void ProcessInput(float dt);
    void Update(float dt);
    void DoCollisions();
    void Render();
};

#endif