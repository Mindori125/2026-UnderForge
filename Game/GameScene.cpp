#include "GameScene.h"

#include "Player.h"
#include "TileMap.h"
#include "Stone.h"

#include "../Engine/Physics/Collision.h"

#include <algorithm>


namespace
{
    constexpr float SCREEN_WIDTH =
        800.0f;

    constexpr float SCREEN_HEIGHT =
        600.0f;


    constexpr float STONE_SPAWN_MARGIN =
        80.0f;
}


GameScene::GameScene()
    : objectManager(nullptr),
      player(nullptr),
      camera(nullptr),
      healthBar(nullptr),
      tileMap(nullptr),
      hoveredStone(nullptr),
      mouseX(0.0f),
      mouseY(0.0f)
{
}


GameScene::~GameScene()
{
    // =========================
    // 기본 마우스 다시 표시
    // =========================

    SDL_ShowCursor();


    // =========================
    // Stones
    // =========================

    for (Stone* stone : stones)
    {
        delete stone;
    }


    stones.clear();

    hoveredStone = nullptr;


    // =========================
    // ObjectManager
    // =========================

    if (objectManager != nullptr)
    {
        delete objectManager;

        objectManager = nullptr;
    }


    player = nullptr;


    // =========================
    // HealthBar
    // =========================

    if (healthBar != nullptr)
    {
        delete healthBar;

        healthBar = nullptr;
    }


    // =========================
    // Camera
    // =========================

    if (camera != nullptr)
    {
        delete camera;

        camera = nullptr;
    }


    // =========================
    // TileMap
    // =========================

    if (tileMap != nullptr)
    {
        delete tileMap;

        tileMap = nullptr;
    }
}


bool GameScene::Initialize(
    SDL_Renderer* renderer
)
{
    // =========================
    // 브라우저 / OS 기본 커서 숨기기
    // =========================

    SDL_HideCursor();


    // =========================
    // Random
    // =========================

    randomEngine.seed(
        static_cast<unsigned int>(
            SDL_GetTicks()
        )
    );


    // =========================
    // ObjectManager
    // =========================

    objectManager =
        new ObjectManager();


    // =========================
    // Camera
    // =========================

    camera =
        new Camera(
            SCREEN_WIDTH,
            SCREEN_HEIGHT
        );


    // =========================
    // HealthBar
    // =========================

    healthBar =
        new HealthBar(
            250.0f,
            550.0f,
            300.0f,
            20.0f
        );


    // =========================
    // TileMap
    // =========================

    tileMap =
        new TileMap();


    if (!tileMap->Load(renderer))
    {
        SDL_Log(
            "Failed to load tile map"
        );

        return false;
    }


    // =========================
    // Stone Texture
    // =========================

    if (!stoneTexture.Load(
            renderer,
            "Assets/Textures/World/stone.png"
        ))
    {
        SDL_Log(
            "Failed to load stone.png"
        );

        return false;
    }


    // =========================
    // Cursor Textures
    // =========================

    if (!defaultCursorTexture.Load(
            renderer,
            "Assets/Textures/UI/cursor_default.png"
        ))
    {
        SDL_Log(
            "Failed to load cursor_default.png"
        );

        return false;
    }


    if (!pickaxeCursorTexture.Load(
            renderer,
            "Assets/Textures/UI/cursor_pickaxe.png"
        ))
    {
        SDL_Log(
            "Failed to load cursor_pickaxe.png"
        );

        return false;
    }


    // =========================
    // Player
    // =========================

    player =
        new Player(
            1008.0f,
            1008.0f,
            50.0f,
            50.0f
        );


    if (!player->LoadTextures(renderer))
    {
        SDL_Log(
            "Failed to load player textures"
        );

        return false;
    }


    objectManager->AddObject(
        player
    );


    // =========================
    // Camera 초기화
    // =========================

    UpdateCamera();


    // =========================
    // 돌 15개 생성
    // =========================

    SpawnInitialStones();


    return true;
}


void GameScene::MovePlayerWithCollisions(
    float deltaX,
    float deltaY
)
{
    if (
        player == nullptr ||
        tileMap == nullptr
    )
    {
        return;
    }


    // =========================
    // X
    // =========================

    if (deltaX != 0.0f)
    {
        SDL_FRect nextHitbox =
            player->GetHitbox();


        nextHitbox.x +=
            deltaX;


        bool blocked =
            tileMap->IsBlocked(
                nextHitbox
            );


        if (!blocked)
        {
            blocked =
                IsPlayerBlockedByStone(
                    nextHitbox
                );
        }


        if (!blocked)
        {
            player->Move(
                deltaX,
                0.0f
            );
        }
    }


    // =========================
    // Y
    // =========================

    if (deltaY != 0.0f)
    {
        SDL_FRect nextHitbox =
            player->GetHitbox();


        nextHitbox.y +=
            deltaY;


        bool blocked =
            tileMap->IsBlocked(
                nextHitbox
            );


        if (!blocked)
        {
            blocked =
                IsPlayerBlockedByStone(
                    nextHitbox
                );
        }


        if (!blocked)
        {
            player->Move(
                0.0f,
                deltaY
            );
        }
    }
}


bool GameScene::IsPlayerBlockedByStone(
    const SDL_FRect& playerHitbox
) const
{
    for (Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        if (
            Collision::CheckAABB(
                playerHitbox,
                stone->GetCollider()
            )
        )
        {
            return true;
        }
    }


    return false;
}


void GameScene::UpdateCamera()
{
    if (
        camera == nullptr ||
        player == nullptr ||
        tileMap == nullptr
    )
    {
        return;
    }


    camera->Follow(
        player->GetX() +
            player->GetBounds().w /
            2.0f,

        player->GetY() +
            player->GetBounds().h /
            2.0f
    );


    float clampedX =
        camera->GetX();


    float clampedY =
        camera->GetY();


    float maxX =
        static_cast<float>(
            tileMap->GetWorldWidth()
        ) -
        SCREEN_WIDTH;


    float maxY =
        static_cast<float>(
            tileMap->GetWorldHeight()
        ) -
        SCREEN_HEIGHT;


    if (maxX < 0.0f)
    {
        maxX = 0.0f;
    }


    if (maxY < 0.0f)
    {
        maxY = 0.0f;
    }


    if (clampedX < 0.0f)
    {
        clampedX = 0.0f;
    }


    if (clampedY < 0.0f)
    {
        clampedY = 0.0f;
    }


    if (clampedX > maxX)
    {
        clampedX = maxX;
    }


    if (clampedY > maxY)
    {
        clampedY = maxY;
    }


    camera->SetPosition(
        clampedX,
        clampedY
    );
}


void GameScene::SpawnInitialStones()
{
    for (Stone* stone : stones)
    {
        delete stone;
    }


    stones.clear();


    int attempts = 0;


    constexpr int MAX_ATTEMPTS =
        5000;


    while (
        static_cast<int>(
            stones.size()
        ) < MAX_STONES &&
        attempts < MAX_ATTEMPTS
    )
    {
        if (!SpawnStoneOutsideView())
        {
            attempts++;
        }
    }


    SDL_Log(
        "Spawned stones: %d",
        static_cast<int>(
            stones.size()
        )
    );
}


bool GameScene::SpawnStoneOutsideView()
{
    if (
        tileMap == nullptr ||
        camera == nullptr ||
        player == nullptr
    )
    {
        return false;
    }


    constexpr float STONE_WIDTH =
        64.0f;


    constexpr float STONE_HEIGHT =
        44.0f;


    float maxX =
        static_cast<float>(
            tileMap->GetWorldWidth()
        ) -
        STONE_WIDTH;


    float maxY =
        static_cast<float>(
            tileMap->GetWorldHeight()
        ) -
        STONE_HEIGHT;


    if (
        maxX <= 0.0f ||
        maxY <= 0.0f
    )
    {
        return false;
    }


    constexpr float WORLD_MARGIN =
        30.0f;


    std::uniform_real_distribution<float>
        xDistribution(
            WORLD_MARGIN,
            maxX - WORLD_MARGIN
        );


    std::uniform_real_distribution<float>
        yDistribution(
            WORLD_MARGIN,
            maxY - WORLD_MARGIN
        );


    constexpr int POSITION_ATTEMPTS =
        100;


    for (
        int attempt = 0;
        attempt < POSITION_ATTEMPTS;
        attempt++
    )
    {
        float x =
            xDistribution(
                randomEngine
            );


        float y =
            yDistribution(
                randomEngine
            );


        Stone* candidate =
            new Stone(
                x,
                y
            );


        SDL_FRect collider =
            candidate->GetCollider();


        SDL_FRect bounds =
            candidate->GetBounds();


        if (!IsOutsideCameraView(
                bounds
            ))
        {
            delete candidate;

            continue;
        }


        if (!IsPositionValidForStone(
                collider,
                bounds
            ))
        {
            delete candidate;

            continue;
        }


        stones.push_back(
            candidate
        );


        return true;
    }


    return false;
}


bool GameScene::IsOutsideCameraView(
    const SDL_FRect& bounds
) const
{
    if (camera == nullptr)
    {
        return true;
    }


    SDL_FRect cameraView;


    cameraView.x =
        camera->GetX() -
        STONE_SPAWN_MARGIN;


    cameraView.y =
        camera->GetY() -
        STONE_SPAWN_MARGIN;


    cameraView.w =
        SCREEN_WIDTH +
        STONE_SPAWN_MARGIN *
        2.0f;


    cameraView.h =
        SCREEN_HEIGHT +
        STONE_SPAWN_MARGIN *
        2.0f;


    return !Collision::CheckAABB(
        bounds,
        cameraView
    );
}


bool GameScene::IsPositionValidForStone(
    const SDL_FRect& stoneCollider,
    const SDL_FRect& stoneBounds
) const
{
    if (
        tileMap == nullptr ||
        player == nullptr
    )
    {
        return false;
    }


    // TileMap Decor와 겹침
    if (
        tileMap->IsBlocked(
            stoneCollider
        )
    )
    {
        return false;
    }


    // Player와 겹침
    if (
        Collision::CheckAABB(
            stoneCollider,
            player->GetHitbox()
        )
    )
    {
        return false;
    }


    // 다른 돌과 겹침
    for (Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        SDL_FRect existingBounds =
            stone->GetBounds();


        SDL_FRect expandedExisting =
            existingBounds;


        constexpr float STONE_GAP =
            10.0f;


        expandedExisting.x -=
            STONE_GAP;


        expandedExisting.y -=
            STONE_GAP;


        expandedExisting.w +=
            STONE_GAP * 2.0f;


        expandedExisting.h +=
            STONE_GAP * 2.0f;


        if (
            Collision::CheckAABB(
                stoneBounds,
                expandedExisting
            )
        )
        {
            return false;
        }
    }


    return true;
}


void GameScene::DestroyStone(
    Stone* stone
)
{
    if (stone == nullptr)
    {
        return;
    }


    // 선택 중인 돌을 파괴하는 경우
    if (hoveredStone == stone)
    {
        hoveredStone = nullptr;
    }


    auto it =
        std::find(
            stones.begin(),
            stones.end(),
            stone
        );


    if (it == stones.end())
    {
        return;
    }


    delete *it;


    stones.erase(
        it
    );


    // =========================
    // 새로운 돌 자동 생성
    // =========================

    int attempts = 0;


    constexpr int MAX_RESPAWN_ATTEMPTS =
        100;


    while (
        static_cast<int>(
            stones.size()
        ) < MAX_STONES &&
        attempts <
            MAX_RESPAWN_ATTEMPTS
    )
    {
        if (SpawnStoneOutsideView())
        {
            break;
        }


        attempts++;
    }
}


void GameScene::UpdateHoveredStone()
{
    hoveredStone = nullptr;


    if (camera == nullptr)
    {
        return;
    }


    // =========================
    // 현재 마우스 화면 좌표
    // =========================

    SDL_GetMouseState(
        &mouseX,
        &mouseY
    );


    // =========================
    // 화면 좌표 → 월드 좌표
    // =========================

    float worldMouseX =
        mouseX +
        camera->GetX();


    float worldMouseY =
        mouseY +
        camera->GetY();


    // =========================
    // 돌 검사
    // =========================

    for (Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        if (
            stone->ContainsPoint(
                worldMouseX,
                worldMouseY
            )
        )
        {
            hoveredStone =
                stone;

            break;
        }
    }
}


void GameScene::Update(
    float deltaTime
)
{
    if (objectManager == nullptr)
    {
        return;
    }


    // =========================
    // Player
    //
    // 걷기 / 달리기 /
    // 스태미나 / 애니메이션
    // =========================

    objectManager->Update(
        deltaTime
    );


    // =========================
    // Movement
    // =========================

    if (player != nullptr)
    {
        float deltaX =
            player->GetMoveDeltaX(
                deltaTime
            );


        float deltaY =
            player->GetMoveDeltaY(
                deltaTime
            );


        MovePlayerWithCollisions(
            deltaX,
            0.0f
        );


        MovePlayerWithCollisions(
            0.0f,
            deltaY
        );
    }


    // =========================
    // Camera
    // =========================

    UpdateCamera();


    // =========================
    // Mouse Hover
    // =========================

    UpdateHoveredStone();
}


void GameScene::RenderStaminaBar(
    SDL_Renderer* renderer
)
{
    if (player == nullptr)
    {
        return;
    }


    float stamina =
        player->GetStamina();


    float maxStamina =
        player->GetMaxStamina();


    if (maxStamina <= 0.0f)
    {
        return;
    }


    // 최대일 때 완전히 숨김
    if (stamina >= maxStamina)
    {
        return;
    }


    float percent =
        stamina /
        maxStamina;


    if (percent < 0.0f)
    {
        percent = 0.0f;
    }


    if (percent > 1.0f)
    {
        percent = 1.0f;
    }


    constexpr float BAR_HEIGHT =
        12.0f;


    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );


    SDL_FRect backgroundRect =
    {
        0.0f,

        SCREEN_HEIGHT -
            BAR_HEIGHT,

        SCREEN_WIDTH,

        BAR_HEIGHT
    };


    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        100
    );


    SDL_RenderFillRect(
        renderer,
        &backgroundRect
    );


    SDL_FRect staminaRect =
    {
        0.0f,

        SCREEN_HEIGHT -
            BAR_HEIGHT,

        SCREEN_WIDTH *
            percent,

        BAR_HEIGHT
    };


    SDL_SetRenderDrawColor(
        renderer,
        255,
        220,
        40,
        255
    );


    SDL_RenderFillRect(
        renderer,
        &staminaRect
    );


    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_NONE
    );
}


void GameScene::RenderCursor(
    SDL_Renderer* renderer
)
{
    // =========================
    // 실제 업로드한 커서 크기
    //
    // Default = 23x22
    // Pickaxe = 23x23
    //
    // 게임에서 조금 더 보기 좋게
    // 32px 정도로 확대
    // =========================

    SDL_FRect cursorRect;


    cursorRect.x =
        mouseX;


    cursorRect.y =
        mouseY;


    // 돌 위에 있으면
    // Pickaxe Cursor
    if (hoveredStone != nullptr)
    {
        cursorRect.w =
            24.0f;

        cursorRect.h =
            24.0f;


        pickaxeCursorTexture.Render(
            renderer,
            cursorRect
        );
    }

    // 일반 Cursor
    else
    {
        cursorRect.w =
            24.0f;

        cursorRect.h =
            24.0f;


        defaultCursorTexture.Render(
            renderer,
            cursorRect
        );
    }
}


void GameScene::Render(
    SDL_Renderer* renderer
)
{
    // =========================
    // 1. TileMap
    // =========================

    if (
        tileMap != nullptr &&
        camera != nullptr
    )
    {
        tileMap->Render(
            renderer,
            *camera
        );
    }


    // =========================
    // 2. Stones
    // =========================

    if (camera != nullptr)
    {
        for (Stone* stone : stones)
        {
            if (stone == nullptr)
            {
                continue;
            }


            // =========================
            // 마우스가 올라간 돌이면
            // 먼저 빨간 Glow 렌더링
            // =========================

            if (stone == hoveredStone)
            {
                stone->RenderHighlight(
                    renderer,
                    *camera,
                    stoneTexture
                );
            }


            // 원본 돌
            stone->Render(
                renderer,
                *camera,
                stoneTexture
            );
        }
    }


    // =========================
    // 3. Player
    // =========================

    if (
        objectManager != nullptr &&
        camera != nullptr
    )
    {
        objectManager->Render(
            renderer,
            *camera
        );
    }


    // =========================
    // 4. Health
    // =========================

    if (
        healthBar != nullptr &&
        player != nullptr
    )
    {
        healthBar->Render(
            renderer,
            player->GetHealth(),
            player->GetMaxHealth()
        );
    }


    // =========================
    // 5. Stamina
    // =========================

    RenderStaminaBar(
        renderer
    );


    // =========================
    // 6. Cursor
    //
    // 항상 가장 마지막에 렌더링해서
    // 모든 게임 오브젝트보다 위에 표시
    // =========================

    RenderCursor(
        renderer
    );
}