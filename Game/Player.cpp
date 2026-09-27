#include "Player.h"
#include "../Engine/Input/Input.h"

Player::Player(float x, float y, float width, float height)
    : GameObject(x, y, width, height),
      speed(300.0f),
      screenWidth(800),
      screenHeight(600) 
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

    if (rect.x < 0)
        rect.x = 0;

    if (rect.y < 0)
        rect.y = 0;

    if (rect.x + rect.w > screenWidth)
        rect.x = screenWidth - rect.w;

    if (rect.y + rect.h > screenHeight)
        rect.y = screenHeight - rect.h;
}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}