#include <iostream>
#include <sstream>
#include "game.h"
#include "sprite_renderer.h"
#include "resource_manager.h"
#include "game_object.h"
#include "text_renderer.h"

SpriteRenderer  *Renderer;
TextRenderer *Text;

float delta = 0.1f;

void ResolveCollision(GameObject &one, GameObject &two) {
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    if (collisionX) one.Position.x -= one.deltaX;
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    if (collisionY) one.Position.y -= one.deltaY;
}

bool CheckCollision(GameObject one, GameObject two) // AABB - AABB collision
{
    // collision x-axis?
    bool collisionX = one.Position.x + one.Size.x >= two.Position.x &&
                      two.Position.x + two.Size.x >= one.Position.x;
    // collision y-axis?
    bool collisionY = one.Position.y + one.Size.y >= two.Position.y &&
                      two.Position.y + two.Size.y >= one.Position.y;
    // collision only if on both axes
    return collisionX && collisionY;
}

Game::Game(unsigned int width, unsigned int height)
        : State(GAME_ACTIVE), Keys(), Width(width), Height(height), PLAYER_VELOCITY(100.0f), ENEMY_VELOCITY(30.0f), score(0), TotalTimePlayed(0.0f)
{

}

Game::~Game()
{
    delete Renderer;
    delete Text;
}

void Game::Init()
{
    ResourceManager::LoadShader("../src/shaders/sprite.vs", "../src/shaders/sprite.frag", nullptr, "sprite");
    glm::mat4 projection = glm::ortho(0.0f, static_cast<float>(this->Width),
                                      static_cast<float>(this->Height), 0.0f, -1.0f, 1.0f);
    ResourceManager::GetShader("sprite").Use().SetInteger("image", 0);
    ResourceManager::GetShader("sprite").SetMatrix4("projection", projection);

    Renderer = new SpriteRenderer(ResourceManager::GetShader("sprite"));
    Text = new TextRenderer(this->Width, this->Height);
    Text->Load("../src/textures/ka1.ttf", 24);
    ResourceManager::LoadTexture("../src/textures/awesomeface.png", true, "face");
    ResourceManager::LoadTexture("../src/textures/awesomeface.png", true, "coin");
    ResourceManager::LoadTexture("../src/textures/paddle.png", true, "wall");
    ResourceManager::LoadTexture("../src/textures/ghost.png", true, "enemy");

    GameLevel level;
    level.init(100, 3, 10, 40, 40);
    Levels.push_back(level);
    level.init(120, 5, 15, 40, 40);
    Levels.push_back(level);
    level.init(140, 7, 20, 40, 40);
    Levels.push_back(level);
    Level = 0;
}

void Game::DoCollisions()
{
//     player-wall
    for(auto wall: this->Levels[this->Level].Walls)
    {
        if (CheckCollision(this->Levels[this->Level].Player, wall))
        ResolveCollision(this->Levels[this->Level].Player, wall);
    }
    // enemy-wall
    for(auto wall: this->Levels[this->Level].Walls)
    {
        for(auto enemy: this->Levels[this->Level].Enemies)
        {
            if (CheckCollision(enemy, wall))
                ResolveCollision(enemy, wall);
        }
    }
    // player-enemy
    for(auto enemy: this->Levels[this->Level].Enemies)
    {
        if(CheckCollision(enemy, this->Levels[this->Level].Player))
        {
            this->State = GAME_LOSE;
        }
    }
    // player-coin
    for (GameObject &coin : this->Levels[this->Level].Coins)
    {
        if (!coin.Destroyed && CheckCollision(this->Levels[this->Level].Player, coin))
        {
            coin.Destroyed = true;
            score += 100;
        }
    }
}

void Game::ProcessInput(float dt)
{
    if (this->State == GAME_ACTIVE)
    {
        float velocity = this->PLAYER_VELOCITY * dt;
        // move playerboard
        float dx = 0.0, dy = 0.0;
        int changes = 0;
        if (this->Keys[GLFW_KEY_A])
        {
            if (this->Levels[this->Level].Player.Position.x >= 0.0f)
            {
                this->Levels[this->Level].Player.Position.x -= velocity;
//                this->Levels[this->Level].Player.deltaX = -1 * delta;
                dx -= velocity;
                changes = 1;
            }
        }
        if (this->Keys[GLFW_KEY_D])
        {
            if (this->Levels[this->Level].Player.Position.x <= this->Width - this->Levels[this->Level].Player.Size.x)
            {
                this->Levels[this->Level].Player.Position.x += velocity;
//                this->Levels[this->Level].Player.deltaX = delta;
                changes = 1;
                dx += velocity;
            }
        }
        if (this->Keys[GLFW_KEY_W])
        {
            if (this->Levels[this->Level].Player.Position.y >= 0.0f)
            {
                this->Levels[this->Level].Player.Position.y -= velocity;
//                this->Levels[this->Level].Player.deltaY = -1 * delta;
                changes = 1;
                dy -= velocity;
            }
        }
        if (this->Keys[GLFW_KEY_S])
        {
            if (this->Levels[this->Level].Player.Position.y <= this->Height - this->Levels[this->Level].Player.Size.y)
            {
                this->Levels[this->Level].Player.Position.y += velocity;
//                this->Levels[this->Level].Player.deltaY = delta;
                changes = 1;
                dy += velocity;
            }
        }
        if (changes) {
            this->Levels[this->Level].Player.deltaX = dx;
            this->Levels[this->Level].Player.deltaY = dy;
        }
    }

}

void Game::Update(float dt)
{
    if (State == GAME_ACTIVE) {
        TotalTimePlayed += dt;
        // move enemies
        float velocity = dt * ENEMY_VELOCITY;
        for (auto &enemy: this->Levels[this->Level].Enemies) {
//        int move = (rand()%3) - 1;
//        enemy.Position.x += 0.8f * dt * PLAYER_VELOCITY * (float)move;
//        move = (rand()%3) - 1;
//        enemy.Position.y += 0.8f * dt * PLAYER_VELOCITY * (float)move;

            float dx = 0, dy = 0;
            int changes = 0;
            if (enemy.Position.x < this->Levels[this->Level].Player.Position.x) {
                enemy.Position.x += velocity;
                dx += velocity;
                changes = 1;
            } else {
                enemy.Position.x -= velocity;
                dx -= velocity;
                changes = 1;
            }
            if (enemy.Position.y < this->Levels[this->Level].Player.Position.y) {
                enemy.Position.y += velocity;
                changes = 1;
                dy += velocity;
            } else {
                changes = 1;
                enemy.Position.y -= velocity;
                dy -= velocity;
            }
            enemy.deltaX = dx, enemy.deltaY = dy;
        }


        if (this->Levels[this->Level].Player.Position.x + this->Levels[this->Level].Player.Size.x > Width||
            this->Levels[this->Level].Player.Position.y + this->Levels[this->Level].Player.Size.y > Height
                )
            State = LEVEL_WIN;

        if (State == LEVEL_WIN && Level < 2) {
            Level++;
            State = GAME_ACTIVE;
        } else if (State == LEVEL_WIN && Level == 2) {
            State = GAME_WIN;
        }
        this->DoCollisions();
    }
}

void Game::Render()
{
    if(this->State == GAME_ACTIVE)
    {
        // draw level
        this->Levels[this->Level].Draw(*Renderer);
        // render text (don't include in postprocessing)
        std::stringstream ss; ss << this->score;
        Text->RenderText("Score: " + ss.str(), 50.0f, 5.0f, 0.5f);
        std::stringstream ss2; ss2 << this->TotalTimePlayed;
        Text->RenderText("Total time played: " + ss2.str(), 500.0f, 5.0f, 0.5f);
    }
    else if(State == GAME_LOSE)
    {
        std::stringstream ss; ss << this->score;
        Text->RenderText("You lose!", 300.0f, 200.0f, 1.0f, glm::vec3(1.0f, 0.0f, 0.0f));
        Text->RenderText("Final Score: " + ss.str(), 250.0f, 300.0f, 1.0f);
        std::stringstream ss2; ss2 << this->TotalTimePlayed;
        Text->RenderText("Total Time played: " + ss2.str(), 200.0f, 400.0f, 0.85f);
    }
    else if(State == GAME_WIN)
    {
        std::stringstream ss; ss << this->score;
        Text->RenderText("You win!", 300.0f, 200.0f, 1.0f, glm::vec3(0.0f, 1.0f, 0.0f));
        Text->RenderText("Final Score: " + ss.str(), 250.0f, 300.0f, 1.0f);
        std::stringstream ss2; ss2 << this->TotalTimePlayed;
        Text->RenderText("Total Time played: " + ss2.str(), 200.0f, 400.0f, 0.85f);
    }
}