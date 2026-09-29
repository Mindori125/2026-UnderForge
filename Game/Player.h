#pragma once

#include "../Engine/Object/GameObject.h"
#include "../Engine/Graphics/Texture.h"
#include "../Engine/Graphics/Animation.h"

class Camera;


class Player : public GameObject
{
public:
    Player(
        float x,
        float y,
        float width,
        float height
    );

    void Update(float deltaTime) override;

    void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    ) override;

    bool LoadTextures(SDL_Renderer* renderer);

    void TakeDamage(int damage);

    int GetHealth() const;
    int GetMaxHealth() const;

    // =========================
    // Stamina
    // =========================

    float GetStamina() const;
    float GetMaxStamina() const;

    bool IsRunning() const;


    // =========================
    // Collision
    // =========================

    SDL_FRect GetHitbox() const;


    // =========================
    // Movement
    // =========================

    float GetMoveDeltaX(float deltaTime) const;
    float GetMoveDeltaY(float deltaTime) const;

    void Move(float dx, float dy);


private:
    // =========================
    // Movement
    // =========================

    float walkSpeed;
    float runMultiplier;

    float moveIntentX;
    float moveIntentY;

    bool isMoving;
    bool isRunning;
    bool facingLeft;


    // =========================
    // Health
    // =========================

    int health;
    int maxHealth;


    // =========================
    // Stamina
    // =========================

    float stamina;
    float maxStamina;

    float staminaDrainRate;
    float staminaRegenRate;

    float staminaRegenDelay;
    float staminaRegenTimer;


    // =========================
    // Rendering
    // =========================

    float renderWidth;
    float renderHeight;

    // 캐릭터 이미지를 아래쪽으로 이동
    float spriteOffsetY;


    // =========================
    // Textures
    // =========================

    Texture idleTexture;
    Texture walkTexture;
    Texture runTexture;


    // =========================
    // Animations
    // =========================

    Animation idleAnimation;
    Animation walkAnimation;
    Animation runAnimation;
};