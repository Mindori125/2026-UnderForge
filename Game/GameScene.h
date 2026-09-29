#pragma once

#include <SDL3/SDL.h>

#include <vector>
#include <random>

#include "../Engine/Scene/Scene.h"
#include "../Engine/Object/ObjectManager.h"
#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/Texture.h"
#include "../Engine/UI/HealthBar.h"


class Player;
class TileMap;
class Stone;


class GameScene : public Scene
{
public:
    GameScene();
    ~GameScene() override;


    bool Initialize(
        SDL_Renderer* renderer
    ) override;


    void Update(
        float deltaTime
    ) override;


    void Render(
        SDL_Renderer* renderer
    ) override;


    // 나중에 채굴 시스템에서 사용
    void DestroyStone(
        Stone* stone
    );


private:
    // =========================
    // Player
    // =========================

    void MovePlayerWithCollisions(
        float deltaX,
        float deltaY
    );


    bool IsPlayerBlockedByStone(
        const SDL_FRect& playerHitbox
    ) const;


    // =========================
    // Camera
    // =========================

    void UpdateCamera();


    // =========================
    // Stone
    // =========================

    void SpawnInitialStones();


    bool SpawnStoneOutsideView();


    bool IsPositionValidForStone(
        const SDL_FRect& stoneCollider,
        const SDL_FRect& stoneBounds
    ) const;


    bool IsOutsideCameraView(
        const SDL_FRect& bounds
    ) const;


    // =========================
    // Mouse / Cursor
    // =========================

    void UpdateHoveredStone();


    void RenderCursor(
        SDL_Renderer* renderer
    );


    // =========================
    // UI
    // =========================

    void RenderStaminaBar(
        SDL_Renderer* renderer
    );


private:
    ObjectManager* objectManager;

    Player* player;

    Camera* camera;

    HealthBar* healthBar;

    TileMap* tileMap;


    // =========================
    // Stone
    // =========================

    Texture stoneTexture;

    std::vector<Stone*> stones;

    std::mt19937 randomEngine;

    static constexpr int MAX_STONES =
        15;


    // =========================
    // Mouse
    // =========================

    Texture defaultCursorTexture;

    Texture pickaxeCursorTexture;


    Stone* hoveredStone;


    float mouseX;
    float mouseY;
};