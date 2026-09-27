#pragma once

#include "../Engine/Object/GameObject.h"

class Camera;

class Enemy : public GameObject
{
public:
    Enemy(float x, float y, float width, float height);

    void Update(float deltaTime) override;
    void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    ) override;
    void SetColliding(bool value);
    
private:
    bool colliding;

};

