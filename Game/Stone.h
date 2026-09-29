#pragma once

#include <SDL3/SDL.h>


class Camera;
class Texture;


class Stone
{
public:
    Stone(
        float x,
        float y
    );


    // 일반 돌 렌더링
    void Render(
        SDL_Renderer* renderer,
        const Camera& camera,
        const Texture& texture
    ) const;


    // 마우스가 올라왔을 때
    // 빨간색 테두리 효과
    void RenderHighlight(
        SDL_Renderer* renderer,
        const Camera& camera,
        const Texture& texture
    ) const;


    const SDL_FRect& GetBounds() const;


    SDL_FRect GetCollider() const;


    // 월드 좌표의 한 점이
    // 돌 위에 있는지 검사
    bool ContainsPoint(
        float worldX,
        float worldY
    ) const;


private:
    SDL_FRect GetSourceRect() const;


private:
    SDL_FRect bounds;
};