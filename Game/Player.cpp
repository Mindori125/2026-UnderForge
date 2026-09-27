#include "Player.h"

Player::Player(float x, float y, float width, float height)
    : rect{x, y, width, height},
      speed(300.0f)
{
}

void Player::Update(float deltaTime)
{
    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    if (keyboard[SDL_SCANCODE_W])
        rect.y -= speed * deltaTime;

    if (keyboard[SDL_SCANCODE_S])
        rect.y += speed * deltaTime;

    if (keyboard[SDL_SCANCODE_A])
        rect.x -= speed * deltaTime;

    if (keyboard[SDL_SCANCODE_D])
        rect.x += speed * deltaTime;

    // 현재 게임 화면 크기: 800 x 600
    if (rect.x < 0)
        rect.x = 0;

    if (rect.y < 0)
        rect.y = 0;

    if (rect.x + rect.w > 800)
        rect.x = 800 - rect.w;

    if (rect.y + rect.h > 600)
        rect.y = 600 - rect.h;
}

void Player::Render(SDL_Renderer* renderer)
{
    SDL_SetRenderDrawColor(renderer, 255, 255, 255, 255);
    SDL_RenderFillRect(renderer, &rect);
}