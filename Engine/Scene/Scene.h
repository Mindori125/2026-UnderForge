#pragma once

#include <SDL3/SDL.h>

class Scene
{
public:
    virtual ~Scene() = default;

    virtual bool Initialize(SDL_Renderer* renderer) = 0;
    virtual void Update(float deltaTime) = 0;
    virtual void Render(SDL_Renderer* renderer) = 0;
};