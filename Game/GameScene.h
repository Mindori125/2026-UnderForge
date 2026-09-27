#pragma once

#include "../Engine/Scene/Scene.h"
#include "../Engine/Object/ObjectManager.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/UI/HealthBar.h"

class Player;
class Enemy;

class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene() override;

    bool Initialize(SDL_Renderer* renderer) override;
    void Update(float deltaTime) override;
    void Render(SDL_Renderer* renderer) override;

private:
    ObjectManager* objectManager;

    Player* player;
    Enemy* enemy;
    Camera* camera;
    HealthBar* healthBar;
};