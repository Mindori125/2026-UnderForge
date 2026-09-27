#include "Camera.h"

Camera::Camera(float width, float height)
    : x(0.0f),
      y(0.0f),
      width(width),
      height(height)
{
}

void Camera::SetPosition(float x, float y)
{
    this->x = x;
    this->y = y;
}

void Camera::Follow(float targetX, float targetY)
{
    x = targetX - width / 2.0f;
    y = targetY - height / 2.0f;
}

SDL_FRect Camera::WorldToScreen(const SDL_FRect& worldRect) const
{
    SDL_FRect screenRect = worldRect;

    screenRect.x -= x;
    screenRect.y -= y;

    return screenRect;
}

float Camera::GetX() const
{
    return x;
}

float Camera::GetY() const
{
    return y;
}