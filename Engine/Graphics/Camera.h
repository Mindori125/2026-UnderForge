#pragma once

#include <SDL3/SDL.h>

class Camera
{
public:
    Camera(float width, float height);

    void SetPosition(float x, float y);
    void Follow(float targetX, float targetY);

    SDL_FRect WorldToScreen(const SDL_FRect& worldRect) const;

    float GetX() const;
    float GetY() const;

private:
    float x;
    float y;

    float width;
    float height;
};