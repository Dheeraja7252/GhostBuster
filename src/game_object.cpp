#include "game_object.h"

GameObject::GameObject()
        : Position(0.0f, 0.0f), Size(1.0f, 1.0f), Velocity(0.0f), Color(1.0f), Sprite(),
        Destroyed(false), ReachedExit(false), deltaX(-5.0f), deltaY(-5.0f) { }

GameObject::GameObject(glm::vec2 pos, glm::vec2 size, Texture2D sprite, glm::vec3 color, glm::vec2 velocity)
        : Position(pos), Size(size), Velocity(velocity), Color(color), Sprite(sprite), Destroyed(false),
        ReachedExit(false), deltaX(-5.0f), deltaY(-5.0f) { }

void GameObject::Draw(SpriteRenderer &renderer)
{
    renderer.DrawSprite(this->Sprite, this->Position, this->Size, 0.0f, this->Color);
}

