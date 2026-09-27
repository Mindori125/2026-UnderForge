#include "GameObject.h"

GameObject::GameObject(float x, float y, float width, float height)
    : rect{x, y, width, height}
{
}

float GameObject::GetX() const
{
    return rect.x;
}

float GameObject::GetY() const
{
    return rect.y;
}