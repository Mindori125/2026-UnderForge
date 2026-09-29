#pragma once

#include <SDL3/SDL.h>
#include <vector>

#include "../Engine/Graphics/Texture.h"

class Camera;

class TileMap
{
public:
    static constexpr int TILE_SIZE = 24;
    static constexpr int MAP_WIDTH_TILES = 84;
    static constexpr int MAP_HEIGHT_TILES = 84;

    TileMap();

    bool Load(SDL_Renderer* renderer);

    void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    ) const;

    bool IsBlocked(const SDL_FRect& rect) const;

    int GetWorldWidth() const;
    int GetWorldHeight() const;

private:
    struct DecorInstance
    {
        SDL_FRect source;
        SDL_FRect destination;
        SDL_FRect collider;
        bool blocks;
    };

    void Generate();

    SDL_FRect IslandSource(
        int col,
        int row
    ) const;

    SDL_FRect DecorSource(
        int col,
        int row,
        int tileWidth = 1,
        int tileHeight = 1
    ) const;

    int Index(int x, int y) const;

    void SetGroundTile(
        int x,
        int y,
        int tileIndex
    );

    void AddDecor(
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
        bool blocks = true
    );

    bool RectsOverlap(
        const SDL_FRect& a,
        const SDL_FRect& b
    ) const;

private:
    int widthTiles;
    int heightTiles;

    std::vector<int> groundTiles;
    std::vector<DecorInstance> decorObjects;

    Texture islandTexture;
    Texture decorTexture;
};