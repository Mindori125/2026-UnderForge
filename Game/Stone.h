#pragma once

#include <SDL3/SDL.h>


class Camera;
class Texture;


class Stone
{
public:
    Stone(
        float x,
        float y,
        int variant
    );


    void Render(
        SDL_Renderer* renderer,
        const Camera& camera,
        const Texture& texture
    ) const;


    void RenderHighlight(
        SDL_Renderer* renderer,
        const Camera& camera,
        const Texture& texture
    ) const;


    const SDL_FRect& GetBounds() const;


    SDL_FRect GetCollider() const;


    bool ContainsPoint(
        float worldX,
        float worldY
    ) const;


    int GetVariant() const;


private:
    SDL_FRect bounds;

    int variant;
};