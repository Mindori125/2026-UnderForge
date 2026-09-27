#pragma once

#include "../Engine/Object/GameObject.h"
#include "../Engine/Graphics/Texture.h"
class Camera;
class Player : public GameObject
{
public:
    Player(float x, float y, float width, float height);

    void Update(float deltaTime) override;
    void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    ) override;

    void SetScreenSize(int width, int height);
    bool LoadTexture(SDL_Renderer* renderer, const std::string& filePath);

    void TakeDamage(int damage);

    int GetHealth() const;
    int GetMaxHealth() const;

private:
    float speed;

    int screenWidth;
    int screenHeight;
    Texture texture;
    int health;
    int maxHealth;
};