#include "Stone.h"

#include "../Engine/Graphics/Camera.h"
#include "../Engine/Graphics/Texture.h"


Stone::Stone(
    float x,
    float y
)
{
    // 게임에서 보이는 돌 크기
    bounds.x = x;
    bounds.y = y;

    bounds.w = 64.0f;
    bounds.h = 44.0f;
}


SDL_FRect Stone::GetSourceRect() const
{
    // =========================
    // 원본 stone.png
    //
    // 600 x 600 이미지에서
    // 실제 돌 부분만 사용
    // =========================

    SDL_FRect sourceRect =
    {
        5.0f,
        68.0f,
        592.0f,
        404.0f
    };


    return sourceRect;
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


    SDL_FRect sourceRect =
        GetSourceRect();


    texture.Render(
        renderer,
        sourceRect,
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


    SDL_FRect sourceRect =
        GetSourceRect();


    // =========================
    // 빨간색 Glow
    //
    // 빨간 돌 실루엣을 여러 방향으로
    // 조금씩 밀어서 먼저 렌더링한다.
    //
    // 이후 원래 돌 이미지를 위에 그리면
    // 가장자리만 빨간색으로 남는다.
    // =========================

    constexpr float GLOW_SIZE =
        2.5f;


    SDL_FRect glowRect =
        screenRect;


    // 왼쪽
    glowRect.x =
        screenRect.x - GLOW_SIZE;

    glowRect.y =
        screenRect.y;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽
    glowRect.x =
        screenRect.x + GLOW_SIZE;

    glowRect.y =
        screenRect.y;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 위
    glowRect.x =
        screenRect.x;

    glowRect.y =
        screenRect.y - GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 아래
    glowRect.x =
        screenRect.x;

    glowRect.y =
        screenRect.y + GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 왼쪽 위
    glowRect.x =
        screenRect.x - GLOW_SIZE;

    glowRect.y =
        screenRect.y - GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽 위
    glowRect.x =
        screenRect.x + GLOW_SIZE;

    glowRect.y =
        screenRect.y - GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 왼쪽 아래
    glowRect.x =
        screenRect.x - GLOW_SIZE;

    glowRect.y =
        screenRect.y + GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
        255,
        30,
        30,
        255
    );


    // 오른쪽 아래
    glowRect.x =
        screenRect.x + GLOW_SIZE;

    glowRect.y =
        screenRect.y + GLOW_SIZE;


    texture.RenderTinted(
        renderer,
        sourceRect,
        glowRect,
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


    collider.w =
        50.0f;

    collider.h =
        28.0f;


    collider.x =
        bounds.x +
        (bounds.w - collider.w) /
        2.0f;


    collider.y =
        bounds.y +
        bounds.h -
        collider.h;


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