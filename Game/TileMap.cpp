#include "TileMap.h"

#include "../Engine/Graphics/Camera.h"


TileMap::TileMap()
    : widthTiles(MAP_WIDTH_TILES),
      heightTiles(MAP_HEIGHT_TILES)
{
}


bool TileMap::Load(SDL_Renderer* renderer)
{
    bool islandLoaded = islandTexture.Load(
        renderer,
        "Assets/Textures/World/island.png"
    );

    bool decorLoaded = decorTexture.Load(
        renderer,
        "Assets/Textures/World/decor.png"
    );

    if (!islandLoaded || !decorLoaded)
    {
        SDL_Log("Failed to load world textures");
        return false;
    }

    Generate();
    return true;
}


void TileMap::Generate()
{
    groundTiles.clear();
    groundTiles.resize(widthTiles * heightTiles, 0);

    decorObjects.clear();

    // island tileset 기준 9열
    const int GRASS_A = 0;   // (0,0)
    const int GRASS_B = 1;   // (1,0)
    const int GRASS_C = 9;   // (0,1)
    const int GRASS_D = 10;  // (1,1)
    const int DIRT    = 4;   // (4,0) 근처의 흙길 타일

    // =========================
    // 1) 전체 Grass 채우기
    // =========================
    for (int y = 0; y < heightTiles; y++)
    {
        for (int x = 0; x < widthTiles; x++)
        {
            int selector = (x * 17 + y * 11) % 4;

            int tile = GRASS_A;

            if (selector == 1) tile = GRASS_B;
            if (selector == 2) tile = GRASS_C;
            if (selector == 3) tile = GRASS_D;

            SetGroundTile(x, y, tile);
        }
    }

    auto paintRect =
        [this](int startX, int startY, int width, int height, int tileIndex)
    {
        for (int y = startY; y < startY + height; y++)
        {
            for (int x = startX; x < startX + width; x++)
            {
                SetGroundTile(x, y, tileIndex);
            }
        }
    };


    // =========================
    // 2) 길 / 광장 배치
    // =========================

    // 중앙 광장
    paintRect(38, 38, 8, 8, DIRT);

    // 중앙 가로길
    paintRect(8, 40, 68, 3, DIRT);

    // 중앙 세로길
    paintRect(40, 8, 3, 68, DIRT);

    // 북동쪽 유적 지역
    paintRect(56, 16, 14, 12, DIRT);

    // 남서쪽 캠프 지역
    paintRect(14, 56, 18, 12, DIRT);

    // 동쪽 광석/탐험 지역 느낌의 넓은 길
    paintRect(58, 46, 14, 10, DIRT);

    // 북서쪽 숲 속 공터
    paintRect(12, 14, 12, 10, DIRT);


    // =========================
    // 3) Decor 배치
    // =========================

    // ---- 큰 나무들 (2x2) ----
    auto addTree = [this](int tx, int ty)
    {
        // decor 시트에서 큰 나무 (대략 2x2)
        AddDecor(
            3, 4, 2, 2,
            tx, ty,
            14.0f, 32.0f, 20.0f, 12.0f,
            true
        );
    };

    // 북서 숲
    addTree(8, 10);
    addTree(13, 8);
    addTree(18, 12);
    addTree(22, 9);
    addTree(24, 14);
    addTree(11, 18);
    addTree(17, 20);

    // 남동 숲
    addTree(60, 60);
    addTree(66, 58);
    addTree(70, 64);
    addTree(62, 68);
    addTree(74, 70);
    addTree(56, 72);

    // 남서 숲 일부
    addTree(10, 66);
    addTree(7, 72);
    addTree(18, 72);

    // ---- 표지판 ----
    auto addSign = [this](int tx, int ty)
    {
        AddDecor(
            3, 2, 1, 1,
            tx, ty,
            4.0f, 10.0f, 16.0f, 10.0f,
            true
        );
    };

    addSign(39, 37);
    addSign(44, 44);
    addSign(20, 55);
    addSign(55, 45);

    // ---- 그루터기 ----
    auto addStump = [this](int tx, int ty)
    {
        AddDecor(
            2, 3, 1, 1,
            tx, ty,
            4.0f, 10.0f, 16.0f, 10.0f,
            true
        );
    };

    addStump(15, 24);
    addStump(21, 21);
    addStump(24, 18);
    addStump(14, 60);
    addStump(24, 64);

    // ---- 바위 ----
    auto addRock = [this](int tx, int ty)
    {
        AddDecor(
            5, 1, 1, 1,
            tx, ty,
            4.0f, 10.0f, 14.0f, 10.0f,
            true
        );
    };

    addRock(30, 16);
    addRock(29, 18);
    addRock(52, 48);
    addRock(67, 52);
    addRock(48, 62);

    // ---- 배럴 ----
    auto addBarrel = [this](int tx, int ty)
    {
        AddDecor(
            1, 1, 1, 1,
            tx, ty,
            3.0f, 8.0f, 18.0f, 16.0f,
            true
        );
    };

    addBarrel(18, 58);
    addBarrel(23, 60);
    addBarrel(65, 22);
    addBarrel(67, 24);

    // ---- 상자 / 상자류 ----
    auto addChest = [this](int tx, int ty)
    {
        AddDecor(
            1, 0, 1, 1,
            tx, ty,
            2.0f, 10.0f, 20.0f, 12.0f,
            true
        );
    };

    addChest(62, 21);
    addChest(21, 59);

    // ---- 횃불 ----
    auto addTorch = [this](int tx, int ty)
    {
        AddDecor(
            1, 4, 1, 1,
            tx, ty,
            7.0f, 12.0f, 10.0f, 10.0f,
            true
        );
    };

    addTorch(58, 19);
    addTorch(68, 19);

    // ---- 모닥불 ----
    AddDecor(
        0, 4, 1, 1,
        16, 61,
        4.0f, 10.0f, 16.0f, 12.0f,
        true
    );

    // ---- 기둥 (1x2) ----
    auto addColumn = [this](int tx, int ty)
    {
        AddDecor(
            6, 0, 1, 2,
            tx, ty,
            3.0f, 18.0f, 18.0f, 28.0f,
            true
        );
    };

    addColumn(58, 18);
    addColumn(68, 18);
    addColumn(58, 24);
    addColumn(68, 24);

    // ---- 루비 / 장식용 보석 (충돌 없음) ----
    AddDecor(
        4, 1, 1, 1,
        26, 58,
        0.0f, 0.0f, 0.0f, 0.0f,
        false
    );

    AddDecor(
        4, 2, 1, 1,
        64, 50,
        0.0f, 0.0f, 0.0f, 0.0f,
        false
    );
}


SDL_FRect TileMap::IslandSource(int col, int row) const
{
    SDL_FRect rect;
    rect.x = static_cast<float>(col * TILE_SIZE);
    rect.y = static_cast<float>(row * TILE_SIZE);
    rect.w = static_cast<float>(TILE_SIZE);
    rect.h = static_cast<float>(TILE_SIZE);
    return rect;
}


SDL_FRect TileMap::DecorSource(
    int col,
    int row,
    int tileWidth,
    int tileHeight
) const
{
    SDL_FRect rect;
    rect.x = static_cast<float>(col * TILE_SIZE);
    rect.y = static_cast<float>(row * TILE_SIZE);
    rect.w = static_cast<float>(tileWidth * TILE_SIZE);
    rect.h = static_cast<float>(tileHeight * TILE_SIZE);
    return rect;
}


int TileMap::Index(int x, int y) const
{
    return y * widthTiles + x;
}


void TileMap::SetGroundTile(int x, int y, int tileIndex)
{
    if (x < 0 || y < 0 || x >= widthTiles || y >= heightTiles)
        return;

    groundTiles[Index(x, y)] = tileIndex;
}


void TileMap::AddDecor(
    int srcCol,
    int srcRow,
    int srcWidthTiles,
    int srcHeightTiles,
    int worldTileX,
    int worldTileY,
    float colliderOffsetX,
    float colliderOffsetY,
    float colliderWidth,
    float colliderHeight,
    bool blocks
)
{
    DecorInstance decor;

    decor.source = DecorSource(
        srcCol,
        srcRow,
        srcWidthTiles,
        srcHeightTiles
    );

    decor.destination.x =
        static_cast<float>(worldTileX * TILE_SIZE);
    decor.destination.y =
        static_cast<float>(worldTileY * TILE_SIZE);
    decor.destination.w =
        static_cast<float>(srcWidthTiles * TILE_SIZE);
    decor.destination.h =
        static_cast<float>(srcHeightTiles * TILE_SIZE);

    decor.collider.x =
        decor.destination.x + colliderOffsetX;
    decor.collider.y =
        decor.destination.y + colliderOffsetY;
    decor.collider.w = colliderWidth;
    decor.collider.h = colliderHeight;

    decor.blocks = blocks;

    decorObjects.push_back(decor);
}


bool TileMap::RectsOverlap(
    const SDL_FRect& a,
    const SDL_FRect& b
) const
{
    if (a.x + a.w <= b.x) return false;
    if (a.x >= b.x + b.w) return false;
    if (a.y + a.h <= b.y) return false;
    if (a.y >= b.y + b.h) return false;

    return true;
}


bool TileMap::IsBlocked(const SDL_FRect& rect) const
{
    // 월드 밖은 이동 불가
    if (rect.x < 0.0f || rect.y < 0.0f)
        return true;

    if (rect.x + rect.w > GetWorldWidth())
        return true;

    if (rect.y + rect.h > GetWorldHeight())
        return true;

    // decor 충돌
    for (const DecorInstance& decor : decorObjects)
    {
        if (!decor.blocks)
            continue;

        if (RectsOverlap(rect, decor.collider))
        {
            return true;
        }
    }

    return false;
}


void TileMap::Render(
    SDL_Renderer* renderer,
    const Camera& camera
) const
{
    const float screenWidth = 800.0f;
    const float screenHeight = 600.0f;

    // =========================
    // Ground 렌더링
    // =========================
    for (int y = 0; y < heightTiles; y++)
    {
        for (int x = 0; x < widthTiles; x++)
        {
            int tileIndex = groundTiles[Index(x, y)];

            int col = tileIndex % 9;
            int row = tileIndex / 9;

            SDL_FRect dest;
            dest.x =
                static_cast<float>(x * TILE_SIZE) - camera.GetX();
            dest.y =
                static_cast<float>(y * TILE_SIZE) - camera.GetY();
            dest.w = static_cast<float>(TILE_SIZE);
            dest.h = static_cast<float>(TILE_SIZE);

            if (dest.x + dest.w < 0.0f ||
                dest.y + dest.h < 0.0f ||
                dest.x > screenWidth ||
                dest.y > screenHeight)
            {
                continue;
            }

            islandTexture.Render(
                renderer,
                IslandSource(col, row),
                dest
            );
        }
    }

    // =========================
    // Decor 렌더링
    // =========================
    for (const DecorInstance& decor : decorObjects)
    {
        SDL_FRect dest = decor.destination;
        dest.x -= camera.GetX();
        dest.y -= camera.GetY();

        if (dest.x + dest.w < 0.0f ||
            dest.y + dest.h < 0.0f ||
            dest.x > screenWidth ||
            dest.y > screenHeight)
        {
            continue;
        }

        decorTexture.Render(
            renderer,
            decor.source,
            dest
        );
    }
}


int TileMap::GetWorldWidth() const
{
    return widthTiles * TILE_SIZE;
}


int TileMap::GetWorldHeight() const
{
    return heightTiles * TILE_SIZE;
}