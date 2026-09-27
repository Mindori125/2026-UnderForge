#pragma once

#include <SDL3/SDL.h>

class Camera;

class GameObject
{
public:
    GameObject(float x, float y, float width, float height);
    virtual ~GameObject() = default;

    virtual void Update(float deltaTime) = 0;

    // 카메라를 받아서 화면 좌표로 렌더링
    virtual void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    ) = 0;

    float GetX() const;
    float GetY() const;

    const SDL_FRect& GetBounds() const;

protected:
    SDL_FRect rect;
};