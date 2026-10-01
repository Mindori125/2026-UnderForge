#pragma once

#include <SDL3/SDL.h>

#include <array>
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


    void DestroyStone(
        Stone* stone
    );


private:

    // =========================
    // Start Screen
    // =========================

    void UpdateStartScreen(
        float deltaTime
    );


    void RenderStartScreen(
        SDL_Renderer* renderer
    );


    void RenderStartText(
        SDL_Renderer* renderer
    );


    void DrawPixelText(
        SDL_Renderer* renderer,
        const char* text,
        float centerX,
        float y,
        float pixelSize
    );


    void DrawPixelCharacter(
        SDL_Renderer* renderer,
        char character,
        float x,
        float y,
        float pixelSize
    );


    bool IsAnyKeyPressed() const;


    // =========================
    // Game
    // =========================

    void MovePlayerWithCollisions(
        float deltaX,
        float deltaY
    );


    bool IsPlayerBlockedByStone(
        const SDL_FRect& playerHitbox
    ) const;


    void UpdateCamera();


    void SpawnInitialStones();


    bool SpawnStoneOutsideView();


    bool IsPositionValidForStone(
        const SDL_FRect& stoneCollider,
        const SDL_FRect& stoneBounds
    ) const;


    bool IsOutsideCameraView(
        const SDL_FRect& bounds
    ) const;


    void UpdateHoveredStone();


    void RenderCursor(
        SDL_Renderer* renderer
    );


    void RenderStaminaBar(
        SDL_Renderer* renderer
    );


private:

    // =========================
    // Start Screen
    // =========================

    bool gameStarted;

    float startScreenTime;

    Texture startBackgroundTexture;

    Texture startIconTexture;


    // =========================
    // Game Objects
    // =========================

    ObjectManager* objectManager;

    Player* player;

    Camera* camera;

    HealthBar* healthBar;

    TileMap* tileMap;


    // =========================
    // Stone
    // =========================

    static constexpr int MAX_STONES =
        15;


    static constexpr int STONE_VARIANT_COUNT =
        5;


    std::array<
        Texture,
        STONE_VARIANT_COUNT
    > stoneTextures;


    std::vector<Stone*> stones;


    std::mt19937 randomEngine;


    // =========================
    // Cursor
    // =========================

    Texture defaultCursorTexture;

    Texture pickaxeCursorTexture;


    Stone* hoveredStone;


    float mouseX;

    float mouseY;
};