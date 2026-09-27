#include "Player.h"
#include "../Engine/Input/Input.h"
#include "../Engine/Graphics/Camera.h"

Player::Player(float x, float y, float width, float height)
    : GameObject(x, y, width, height),
      speed(300.0f),
      screenWidth(800),
      screenHeight(600),
      health(100),
      maxHealth(100)
{
}

void Player::SetScreenSize(int width, int height)
{
    screenWidth = width;
    screenHeight = height;
}

void Player::Update(float deltaTime)
{
    if (Input::IsKeyDown(Key::W))
        rect.y -= speed * deltaTime;

    if (Input::IsKeyDown(Key::S))
        rect.y += speed * deltaTime;

    if (Input::IsKeyDown(Key::A))
        rect.x -= speed * deltaTime;

    if (Input::IsKeyDown(Key::D))
        rect.x += speed * deltaTime;
}

void Player::Render(
    SDL_Renderer* renderer,
    const Camera& camera
)
{
    SDL_FRect screenRect =
        camera.WorldToScreen(rect);

    if (texture.IsLoaded())
    {
        texture.Render(
            renderer,
            screenRect
        );
    }
    else
    {
        SDL_SetRenderDrawColor(
            renderer,
            255,
            255,
            255,
            255
        );

        SDL_RenderFillRect(
            renderer,
            &screenRect
        );
    }
}

bool Player::LoadTexture(
    SDL_Renderer* renderer,
    const std::string& filePath
)
{
    return texture.Load(renderer, filePath);
}   

void Player::TakeDamage(int damage)
{
    health -= damage;

    if (health < 0)
    {
        health = 0;
    }
}

int Player::GetHealth() const
{
    return health;
}

int Player::GetMaxHealth() const
{
    return maxHealth;
}