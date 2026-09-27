#pragma once

#include <SDL3/SDL.h>

class GameObject
{
public:
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject() = default;

    virtual void Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;

    float GetX() const;
    float GetY() const;

protected:
    SDL_FRect rect;
};