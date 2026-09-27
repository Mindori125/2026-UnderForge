#pragma once

#include "../Engine/Object/GameObject.h"

class Player : public GameObject
{
public:
    Player(float x, float y, float width, float height);

    void Update(float deltaTime) override;
    void Render(SDL_Renderer* renderer) override;

    void SetScreenSize(int width, int height);

private:
    float speed;

    int screenWidth;
    int screenHeight;
};