#include "Stone.h"

#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/Texture.h"


Stone::Stone(
    float x,
    float y,
    int variant
)
    : bounds{
          x,
          y,
          64.0f,
          64.0f
      },
      variant(variant)
{
}


void Stone::Render(
    SDL_Renderer* renderer,
    const Camera& camera,
    const Texture& texture
) const
{
    SDL_FRect screenRect =
        camera.WorldToScreen(
            bounds
        );


    texture.Render(
        renderer,
        screenRect
    );
}


void Stone::RenderHighlight(
    SDL_Renderer* renderer,
    const Camera& camera,
    const Texture& texture
) const
{
    SDL_FRect screenRect =
        camera.WorldToScreen(
            bounds
        );


    const float offset =
        2.5f;


    // 왼쪽
    SDL_FRect highlightRect =
        screenRect;

    highlightRect.x -=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽
    highlightRect =
        screenRect;

    highlightRect.x +=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 위
    highlightRect =
        screenRect;

    highlightRect.y -=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 아래
    highlightRect =
        screenRect;

    highlightRect.y +=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 왼쪽 위
    highlightRect =
        screenRect;

    highlightRect.x -=
        offset;

    highlightRect.y -=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽 위
    highlightRect =
        screenRect;

    highlightRect.x +=
        offset;

    highlightRect.y -=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 왼쪽 아래
    highlightRect =
        screenRect;

    highlightRect.x -=
        offset;

    highlightRect.y +=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽 아래
    highlightRect =
        screenRect;

    highlightRect.x +=
        offset;

    highlightRect.y +=
        offset;

    texture.RenderTinted(
        renderer,
        highlightRect,
        255,
        30,
        30,
        255
    );
}


const SDL_FRect& Stone::GetBounds() const
{
    return bounds;
}


SDL_FRect Stone::GetCollider() const
{
    SDL_FRect collider;


    // 돌의 아래쪽 부분만 충돌 판정
    collider.w =
        48.0f;

    collider.h =
        22.0f;


    collider.x =
        bounds.x +
        (bounds.w - collider.w) / 2.0f;


    collider.y =
        bounds.y +
        bounds.h -
        collider.h -
        4.0f;


    return collider;
}


bool Stone::ContainsPoint(
    float worldX,
    float worldY
) const
{
    return
        worldX >= bounds.x &&
        worldX <= bounds.x + bounds.w &&
        worldY >= bounds.y &&
        worldY <= bounds.y + bounds.h;
}


int Stone::GetVariant() const
{
    return variant;
}