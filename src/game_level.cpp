#include "game_level.h"

#include <fstream>
#include <sstream>
#include <iostream>

void GameLevel::Draw(SpriteRenderer &renderer)
{
    for(auto wall: this->Walls)
        wall.Draw(renderer);
    for(auto coin: this->Coins)
        if(!coin.Destroyed)
            coin.Draw(renderer);
    for(auto enemy: this->Enemies)
        enemy.Draw(renderer);
    Player.Draw(renderer);
}

bool GameLevel::IsCompleted()
{
    return (Player.ReachedExit || Player.Destroyed);
}

void GameLevel::init(int numWalls, int numEnemies, int numCoins, int gridSizeX, int gridSizeY)
{
    // grid:
    // 0 -> empty
    // 1 -> wall
    // 2 -> enemy
    // 3 -> coin
    // 4 -> player

    int Width = 800, Height = 600;

    glm::vec3 wallColour = glm::vec3(0.0f, 0.0f, 1.0f);
    glm::vec3 enemyColour = glm::vec3(1.0f);
    glm::vec3 coinColour = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 playerColour = glm::vec3(1.0f, 1.0f, 1.0f);

    glm::vec3 size = glm::vec3(Width/(float) gridSizeX, Height/(float)gridSizeY, 0.0f);
    std::cout << size.x << "  " << size.y << " " << size.z << std::endl;

    std::vector<std::vector<int>> grid(gridSizeX, std::vector<int>(gridSizeY, 0));

    grid[1][1] = 4;
    Player = GameObject(size, 0.9f * size, ResourceManager::GetTexture("face"), playerColour);

    for(int i=0; i<gridSizeY; i++)
    {
        grid[0][i] = grid[gridSizeX - 1][i] = 1;
    }

    for(int i=0; i<gridSizeX; i++)
    {
        grid[i][0] = grid[i][gridSizeY - 1] = 1;
    }

    for(int i=0; i<numWalls; i++)
    {
        while(1)
        {
            int x = rand() % (gridSizeX);
            int y = rand() % (gridSizeY);
            if(grid[x][y] == 0)
            {
                grid[x][y] = 1;
                break;
            }
        }
    }

    for(int i=0; i<numEnemies; i++)
    {
        while(1)
        {
            int x = rand() % (gridSizeX);
            int y = rand() % (gridSizeY);
            if(grid[x][y] == 0)
            {
                grid[x][y] = 2;
                glm::vec3 pos = glm::vec3(size.x * (float)x, size.y * (float)y, 0.0f);
                GameObject obj = GameObject(pos, size * 2.0f, ResourceManager::GetTexture("enemy"), enemyColour);
                Enemies.push_back(obj);
                break;
            }
        }
    }

    for(int i=0; i<numCoins; i++)
    {
        while(1)
        {
            int x = rand() % (gridSizeX);
            int y = rand() % (gridSizeY);
            if(grid[x][y] == 0)
            {
                grid[x][y] = 3;
                glm::vec3 pos = glm::vec3(size.x * (float)x, size.y * (float)y, 0.0f);
                GameObject obj = GameObject(pos, 0.7f * size, ResourceManager::GetTexture("coin"), coinColour);
                Coins.push_back(obj);
                break;
            }
        }
    }

    grid[gridSizeX-1][gridSizeY-2] = grid[gridSizeX-1][gridSizeY-3] = -1;
    grid[gridSizeX-2][gridSizeY-2] = grid[gridSizeX-1][gridSizeY-3] = -1;

    for(int i=0; i<gridSizeX; i++)
    {
        for(int j=0; j<gridSizeY; j++)
        {
            if(grid[i][j] == 1)
            {
                glm::vec2 pos = glm::vec2(size.x * (float)i, size.y * (float)j);
                GameObject obj = GameObject(pos, size, ResourceManager::GetTexture("wall"), wallColour);
                Walls.push_back(obj);
            }
        }
    }

    for(auto row: grid)
    {

        for(auto e: row)
            std::cout << e <<" ";
        std::cout << std::endl;
    }
}

