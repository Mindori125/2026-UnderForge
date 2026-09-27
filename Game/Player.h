#pragma once

#include <SDL3/SDL.h>

class Player
{
public:
    Player(float x, float y, float width, float height);

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

private:
    SDL_FRect rect;
    float speed;
};