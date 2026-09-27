#pragma once

#include <SDL3/SDL.h>

class HealthBar
{
public:
    HealthBar(
        float x,
        float y,
        float width,
        float height
    );

    void Render(
        SDL_Renderer* renderer,
        int currentHealth,
        int maxHealth
    );

private:
    float x;
    float y;
    float width;
    float height;
};