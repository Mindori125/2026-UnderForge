#include "Enemy.h"
#include "../Engine/Graphics/Camera.h"

Enemy::Enemy(float x, float y, float width, float height)
    : GameObject(x, y, width, height),
      colliding(false)
{
}   

void Enemy::Update(float deltaTime)
{
    // 아직 움직임 없음
}

void Enemy::Render(
    SDL_Renderer* renderer,
    const Camera& camera
)
{
    SDL_FRect screenRect =
        camera.WorldToScreen(rect);

    if (colliding)
    {
        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            0,
            255
        );
    }
    else
    {
        SDL_SetRenderDrawColor(
            renderer,
            255,
            60,
            60,
            255
        );
    }

    SDL_RenderFillRect(
        renderer,
        &screenRect
    );
}

void Enemy::SetColliding(bool value)
{
    colliding = value;
}