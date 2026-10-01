#include "GameScene.h"

#include "Player.h"
#include "TileMap.h"
#include "Stone.h"

#include "../Engine/Physics/Collision.h"

#include <algorithm>
#include <cmath>
#include <cstring>


GameScene::GameScene()
    : gameStarted(false),
      startScreenTime(0.0f),
      objectManager(nullptr),
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
    SDL_ShowCursor();


    for (Stone* stone : stones)
    {
        delete stone;
    }


    stones.clear();


    delete objectManager;
    objectManager = nullptr;


    delete healthBar;
    healthBar = nullptr;


    delete camera;
    camera = nullptr;


    delete tileMap;
    tileMap = nullptr;


    player = nullptr;

    hoveredStone = nullptr;
}


bool GameScene::Initialize(
    SDL_Renderer* renderer
)
{
    SDL_HideCursor();


    // =========================
    // 랜덤 시드
    // =========================

    randomEngine.seed(
        static_cast<unsigned int>(
            SDL_GetTicks()
        )
    );


    // =========================
    // Start Screen
    // =========================

    if (!startBackgroundTexture.Load(
            renderer,
            "Assets/Textures/UI/underforge_bg.png"
        ))
    {
        SDL_Log(
            "Failed to load underforge_bg.png"
        );

        return false;
    }


    if (!startIconTexture.Load(
            renderer,
            "Assets/Textures/UI/underforge_icon.png"
        ))
    {
        SDL_Log(
            "Failed to load underforge_icon.png"
        );

        return false;
    }


    // =========================
    // Object Manager
    // =========================

    objectManager =
        new ObjectManager();


    // =========================
    // Camera
    // =========================

    camera =
        new Camera(
            800.0f,
            600.0f
        );


    // =========================
    // Health Bar
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
            "Failed to load TileMap."
        );

        return false;
    }

    // =========================
    // Stone Textures
    // =========================

    const char* stonePaths[
        STONE_VARIANT_COUNT
    ] =
    {
        "Assets/Textures/World/stone_1.png",
        "Assets/Textures/World/stone_2.png",
        "Assets/Textures/World/stone_3.png",
        "Assets/Textures/World/stone_4.png",
        "Assets/Textures/World/stone_5.png"
    };


for (
    int i = 0;
    i < STONE_VARIANT_COUNT;
    ++i
)
{
    if (!stoneTextures[i].Load(
            renderer,
            stonePaths[i]
        ))
    {
        SDL_Log(
            "Failed to load stone texture: %s",
            stonePaths[i]
        );

        return false;
    }
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
            "Failed to load default cursor."
        );

        return false;
    }


    if (!pickaxeCursorTexture.Load(
            renderer,
            "Assets/Textures/UI/cursor_pickaxe.png"
        ))
    {
        SDL_Log(
            "Failed to load pickaxe cursor."
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


    if (!player->LoadTextures(
            renderer
        ))
    {
        SDL_Log(
            "Failed to load Player textures."
        );

        delete player;

        player = nullptr;

        return false;
    }


    objectManager->AddObject(
        player
    );


    // =========================
    // Camera
    // =========================

    UpdateCamera();


    // =========================
    // Stones
    // =========================

    SpawnInitialStones();


    return true;
}


// ============================================================
// START SCREEN
// ============================================================

bool GameScene::IsAnyKeyPressed() const
{
    int keyCount = 0;


    const bool* keyboard =
        SDL_GetKeyboardState(
            &keyCount
        );


    for (
        int i = 0;
        i < keyCount;
        ++i
    )
    {
        if (keyboard[i])
        {
            return true;
        }
    }


    return false;
}


void GameScene::UpdateStartScreen(
    float deltaTime
)
{
    startScreenTime +=
        deltaTime;


    // 아무 키나 누르면 게임 시작
    if (IsAnyKeyPressed())
    {
        gameStarted =
            true;
    }
}


void GameScene::RenderStartScreen(
    SDL_Renderer* renderer
)
{
    // ========================================================
    // 배경
    //
    // 원본:
    // 1672 x 941
    //
    // 게임:
    // 800 x 600
    //
    // 원본을 억지로 4:3으로 찌그러뜨리지 않고
    // 가운데 부분을 잘라서 800x600에 맞춤
    // ========================================================

    const float backgroundSourceHeight =
        941.0f;


    const float backgroundSourceWidth =
        backgroundSourceHeight *
        (800.0f / 600.0f);


    const float backgroundSourceX =
        (1672.0f -
         backgroundSourceWidth)
        / 2.0f;


    SDL_FRect backgroundSource =
    {
        backgroundSourceX,
        0.0f,
        backgroundSourceWidth,
        backgroundSourceHeight
    };


    SDL_FRect backgroundDestination =
    {
        0.0f,
        0.0f,
        800.0f,
        600.0f
    };


    startBackgroundTexture.Render(
        renderer,
        backgroundSource,
        backgroundDestination
    );


    // ========================================================
    // UNDERFORGE ICON
    // ========================================================

    // 천천히 위아래로 움직임
    float floatingOffset =
        std::sin(
            startScreenTime * 1.8f
        ) *
        5.0f;


    // 아이콘 크기
    const float iconWidth =
        500.0f;


    // 원본 비율 유지
    const float iconHeight =
        iconWidth *
        (907.0f / 1734.0f);


    SDL_FRect iconRect;


    iconRect.w =
        iconWidth;


    iconRect.h =
        iconHeight;


    iconRect.x =
        (800.0f -
         iconRect.w)
        / 2.0f;


    // 화면 중앙보다 약간 위
    iconRect.y =
        95.0f +
        floatingOffset;


    startIconTexture.Render(
        renderer,
        iconRect
    );


    // ========================================================
    // PRESS ANY BUTTON TO START
    // ========================================================

    RenderStartText(
        renderer
    );
}


void GameScene::RenderStartText(
    SDL_Renderer* renderer
)
{
    // ========================================================
    // 글씨 크기가
    // 커졌다 → 작아졌다 → 커졌다 반복
    // ========================================================

    float pulse =
        (
            std::sin(
                startScreenTime * 3.0f
            )
            + 1.0f
        )
        * 0.5f;


    float pixelSize =
        3.0f +
        pulse * 0.45f;


    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );


    // 약간의 그림자
    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        170
    );


    DrawPixelText(
        renderer,
        "PRESS ANY BUTTON TO START",
        402.0f,
        503.0f,
        pixelSize
    );


    // 흰색 픽셀 글씨
    SDL_SetRenderDrawColor(
        renderer,
        255,
        255,
        255,
        255
    );


    DrawPixelText(
        renderer,
        "PRESS ANY BUTTON TO START",
        400.0f,
        500.0f,
        pixelSize
    );


    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_NONE
    );
}


void GameScene::DrawPixelText(
    SDL_Renderer* renderer,
    const char* text,
    float centerX,
    float y,
    float pixelSize
)
{
    int length =
        static_cast<int>(
            std::strlen(text)
        );


    // 글자 하나:
    // 5픽셀 너비 + 1픽셀 간격
    float characterWidth =
        6.0f *
        pixelSize;


    float totalWidth =
        static_cast<float>(
            length
        ) *
        characterWidth;


    // 마지막 글자의 뒤쪽 간격 제거
    totalWidth -=
        pixelSize;


    float startX =
        centerX -
        totalWidth / 2.0f;


    float currentX =
        startX;


    for (
        int i = 0;
        i < length;
        ++i
    )
    {
        DrawPixelCharacter(
            renderer,
            text[i],
            currentX,
            y,
            pixelSize
        );


        currentX +=
            characterWidth;
    }
}


void GameScene::DrawPixelCharacter(
    SDL_Renderer* renderer,
    char character,
    float x,
    float y,
    float pixelSize
)
{
    // 5 x 7 픽셀 폰트

    const char* pattern[7] =
    {
        "00000",
        "00000",
        "00000",
        "00000",
        "00000",
        "00000",
        "00000"
    };


    switch (character)
    {
        case 'A':
        {
            static const char* p[7] =
            {
                "01110",
                "10001",
                "10001",
                "11111",
                "10001",
                "10001",
                "10001"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'B':
        {
            static const char* p[7] =
            {
                "11110",
                "10001",
                "10001",
                "11110",
                "10001",
                "10001",
                "11110"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'E':
        {
            static const char* p[7] =
            {
                "11111",
                "10000",
                "10000",
                "11110",
                "10000",
                "10000",
                "11111"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'N':
        {
            static const char* p[7] =
            {
                "10001",
                "11001",
                "11001",
                "10101",
                "10011",
                "10011",
                "10001"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'O':
        {
            static const char* p[7] =
            {
                "01110",
                "10001",
                "10001",
                "10001",
                "10001",
                "10001",
                "01110"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'P':
        {
            static const char* p[7] =
            {
                "11110",
                "10001",
                "10001",
                "11110",
                "10000",
                "10000",
                "10000"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'R':
        {
            static const char* p[7] =
            {
                "11110",
                "10001",
                "10001",
                "11110",
                "10100",
                "10010",
                "10001"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'S':
        {
            static const char* p[7] =
            {
                "01111",
                "10000",
                "10000",
                "01110",
                "00001",
                "00001",
                "11110"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'T':
        {
            static const char* p[7] =
            {
                "11111",
                "00100",
                "00100",
                "00100",
                "00100",
                "00100",
                "00100"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'U':
        {
            static const char* p[7] =
            {
                "10001",
                "10001",
                "10001",
                "10001",
                "10001",
                "10001",
                "01110"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case 'Y':
        {
            static const char* p[7] =
            {
                "10001",
                "10001",
                "01010",
                "00100",
                "00100",
                "00100",
                "00100"
            };

            for (int i = 0; i < 7; ++i)
                pattern[i] = p[i];

            break;
        }


        case ' ':
        {
            return;
        }
    }


    for (
        int row = 0;
        row < 7;
        ++row
    )
    {
        for (
            int column = 0;
            column < 5;
            ++column
        )
        {
            if (
                pattern[row][column] ==
                '1'
            )
            {
                SDL_FRect pixel =
                {
                    x +
                    static_cast<float>(
                        column
                    ) *
                    pixelSize,

                    y +
                    static_cast<float>(
                        row
                    ) *
                    pixelSize,

                    pixelSize,
                    pixelSize
                };


                SDL_RenderFillRect(
                    renderer,
                    &pixel
                );
            }
        }
    }
}


// ============================================================
// GAME
// ============================================================

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
    // X축
    // =========================

    if (deltaX != 0.0f)
    {
        SDL_FRect currentHitbox =
            player->GetHitbox();


        SDL_FRect nextHitbox =
            currentHitbox;


        nextHitbox.x +=
            deltaX;


        if (
            !tileMap->IsBlocked(
                nextHitbox
            ) &&
            !IsPlayerBlockedByStone(
                nextHitbox
            )
        )
        {
            player->Move(
                deltaX,
                0.0f
            );
        }
    }


    // =========================
    // Y축
    // =========================

    if (deltaY != 0.0f)
    {
        SDL_FRect currentHitbox =
            player->GetHitbox();


        SDL_FRect nextHitbox =
            currentHitbox;


        nextHitbox.y +=
            deltaY;


        if (
            !tileMap->IsBlocked(
                nextHitbox
            ) &&
            !IsPlayerBlockedByStone(
                nextHitbox
            )
        )
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
    for (const Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        SDL_FRect stoneCollider =
            stone->GetCollider();


        if (
            Collision::CheckAABB(
                playerHitbox,
                stoneCollider
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


    SDL_FRect playerBounds =
        player->GetBounds();


    float targetX =
        playerBounds.x +
        playerBounds.w / 2.0f;


    float targetY =
        playerBounds.y +
        playerBounds.h / 2.0f;


    camera->Follow(
        targetX,
        targetY
    );


    float cameraX =
        camera->GetX();


    float cameraY =
        camera->GetY();


    const float screenWidth =
        800.0f;


    const float screenHeight =
        600.0f;


    float maxCameraX =
        static_cast<float>(
            tileMap->GetWorldWidth()
        ) -
        screenWidth;


    float maxCameraY =
        static_cast<float>(
            tileMap->GetWorldHeight()
        ) -
        screenHeight;


    if (maxCameraX < 0.0f)
    {
        maxCameraX =
            0.0f;
    }


    if (maxCameraY < 0.0f)
    {
        maxCameraY =
            0.0f;
    }


    cameraX =
        std::clamp(
            cameraX,
            0.0f,
            maxCameraX
        );


    cameraY =
        std::clamp(
            cameraY,
            0.0f,
            maxCameraY
        );


    camera->SetPosition(
        cameraX,
        cameraY
    );
}


void GameScene::SpawnInitialStones()
{
    for (Stone* stone : stones)
    {
        delete stone;
    }


    stones.clear();


    hoveredStone =
        nullptr;


    int attempts =
        0;


    const int maxAttempts =
        5000;


    while (
        static_cast<int>(
            stones.size()
        ) < MAX_STONES &&
        attempts < maxAttempts
    )
    {
        SpawnStoneOutsideView();

        ++attempts;
    }
}


bool GameScene::SpawnStoneOutsideView()
{
    if (
        tileMap == nullptr ||
        player == nullptr ||
        camera == nullptr
    )
    {
        return false;
    }


    const float stoneWidth =
        64.0f;


    const float stoneHeight =
        64.0f;


    const int margin =
        30;


    int maxX =
        tileMap->GetWorldWidth() -
        static_cast<int>(
            stoneWidth
        ) -
        margin;


    int maxY =
        tileMap->GetWorldHeight() -
        static_cast<int>(
            stoneHeight
        ) -
        margin;


    if (
        maxX <= margin ||
        maxY <= margin
    )
    {
        return false;
    }


    std::uniform_int_distribution<int>
        xDistribution(
            margin,
            maxX
        );


    std::uniform_int_distribution<int>
        yDistribution(
            margin,
            maxY
        );


    std::uniform_int_distribution<int>
        variantDistribution(
            0,
            STONE_VARIANT_COUNT - 1
        );


    const int maxAttempts =
        100;


    for (
        int attempt = 0;
        attempt < maxAttempts;
        ++attempt
    )
    {
        float x =
            static_cast<float>(
                xDistribution(
                    randomEngine
                )
            );


        float y =
            static_cast<float>(
                yDistribution(
                    randomEngine
                )
            );


        SDL_FRect stoneBounds =
        {
            x,
            y,
            stoneWidth,
            stoneHeight
        };


        if (
            !IsOutsideCameraView(
                stoneBounds
            )
        )
        {
            continue;
        }


        int variant =
            variantDistribution(
                randomEngine
            );


        Stone* newStone =
            new Stone(
                x,
                y,
                variant
            );


        SDL_FRect stoneCollider =
            newStone->GetCollider();


        if (
            !IsPositionValidForStone(
                stoneCollider,
                stoneBounds
            )
        )
        {
            delete newStone;

            continue;
        }


        stones.push_back(
            newStone
        );


        return true;
    }


    return false;
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


    if (
        tileMap->IsBlocked(
            stoneCollider
        )
    )
    {
        return false;
    }


    SDL_FRect playerHitbox =
        player->GetHitbox();


    if (
        Collision::CheckAABB(
            playerHitbox,
            stoneCollider
        )
    )
    {
        return false;
    }


    const float spacing =
        10.0f;


    SDL_FRect expandedBounds =
        stoneBounds;


    expandedBounds.x -=
        spacing;


    expandedBounds.y -=
        spacing;


    expandedBounds.w +=
        spacing * 2.0f;


    expandedBounds.h +=
        spacing * 2.0f;


    for (const Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        if (
            Collision::CheckAABB(
                expandedBounds,
                stone->GetBounds()
            )
        )
        {
            return false;
        }
    }


    return true;
}


bool GameScene::IsOutsideCameraView(
    const SDL_FRect& bounds
) const
{
    if (camera == nullptr)
    {
        return true;
    }


    const float spawnMargin =
        80.0f;


    SDL_FRect cameraView =
    {
        camera->GetX() -
            spawnMargin,

        camera->GetY() -
            spawnMargin,

        800.0f +
            spawnMargin * 2.0f,

        600.0f +
            spawnMargin * 2.0f
    };


    return !Collision::CheckAABB(
        bounds,
        cameraView
    );
}


void GameScene::UpdateHoveredStone()
{
    hoveredStone =
        nullptr;


    if (camera == nullptr)
    {
        return;
    }


    SDL_GetMouseState(
        &mouseX,
        &mouseY
    );


    float worldMouseX =
        mouseX +
        camera->GetX();


    float worldMouseY =
        mouseY +
        camera->GetY();


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
    // =========================
    // 시작화면
    // =========================

    if (!gameStarted)
    {
        UpdateStartScreen(
            deltaTime
        );

        return;
    }


    // =========================
    // 실제 게임
    // =========================

    if (objectManager != nullptr)
    {
        objectManager->Update(
            deltaTime
        );
    }


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
            deltaY
        );
    }


    UpdateCamera();


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


    if (stamina >= maxStamina)
    {
        return;
    }


    float percent =
        stamina /
        maxStamina;


    percent =
        std::clamp(
            percent,
            0.0f,
            1.0f
        );


    const float screenWidth =
        800.0f;


    const float barHeight =
        12.0f;


    const float barY =
        600.0f -
        barHeight;


    SDL_SetRenderDrawBlendMode(
        renderer,
        SDL_BLENDMODE_BLEND
    );


    SDL_SetRenderDrawColor(
        renderer,
        0,
        0,
        0,
        100
    );


    SDL_FRect backgroundRect =
    {
        0.0f,
        barY,
        screenWidth,
        barHeight
    };


    SDL_RenderFillRect(
        renderer,
        &backgroundRect
    );


    SDL_SetRenderDrawColor(
        renderer,
        255,
        220,
        0,
        255
    );


    SDL_FRect staminaRect =
    {
        0.0f,
        barY,
        screenWidth * percent,
        barHeight
    };


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
    SDL_FRect cursorRect;


    cursorRect.x =
        mouseX;


    cursorRect.y =
        mouseY;


    cursorRect.w =
        20.0f;


    cursorRect.h =
        20.0f;


    if (hoveredStone != nullptr)
    {
        pickaxeCursorTexture.Render(
            renderer,
            cursorRect
        );
    }
    else
    {
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
    if (renderer == nullptr)
    {
        return;
    }


    // =========================
    // 시작화면
    // =========================

    if (!gameStarted)
    {
        RenderStartScreen(
            renderer
        );

        return;
    }


    // =========================
    // 실제 게임
    // =========================

    if (camera == nullptr)
    {
        return;
    }


    // TileMap
    if (tileMap != nullptr)
    {
        tileMap->Render(
            renderer,
            *camera
        );
    }


    // Stones
    for (Stone* stone : stones)
    {
        if (stone == nullptr)
        {
            continue;
        }


        int variant =
            stone->GetVariant();


        if (
            variant < 0 ||
            variant >= STONE_VARIANT_COUNT
        )
        {
            continue;
        }


        const Texture& stoneTexture =
            stoneTextures[variant];


        if (stone == hoveredStone)
        {
            stone->RenderHighlight(
                renderer,
                *camera,
                stoneTexture
            );
        }


        stone->Render(
            renderer,
            *camera,
            stoneTexture
        );
    }


    // Player
    if (objectManager != nullptr)
    {
        objectManager->Render(
            renderer,
            *camera
        );
    }


    // Health
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


    // Stamina
    RenderStaminaBar(
        renderer
    );


    // Cursor
    RenderCursor(
        renderer
    );
}


void GameScene::DestroyStone(
    Stone* stone
)
{
    if (stone == nullptr)
    {
        return;
    }


    if (hoveredStone == stone)
    {
        hoveredStone =
            nullptr;
    }


    auto iterator =
        std::find(
            stones.begin(),
            stones.end(),
            stone
        );


    if (
        iterator !=
        stones.end()
    )
    {
        delete *iterator;


        stones.erase(
            iterator
        );
    }


    // =========================
    // 돌 최대 15개 유지
    // =========================

    int attempts =
        0;


    const int maxAttempts =
        500;


    while (
        static_cast<int>(
            stones.size()
        ) < MAX_STONES &&
        attempts < maxAttempts
    )
    {
        SpawnStoneOutsideView();

        ++attempts;
    }
}