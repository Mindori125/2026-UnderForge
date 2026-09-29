#pragma once

#include <SDL3/SDL.h>
#include <string>


class Texture
{
public:
    Texture();
    ~Texture();


    bool Load(
        SDL_Renderer* renderer,
        const std::string& filePath
    );


    // 일반 렌더링
    void Render(
        SDL_Renderer* renderer,
        const SDL_FRect& destination
    ) const;


    // Sprite Sheet 렌더링
    void Render(
        SDL_Renderer* renderer,
        const SDL_FRect& source,
        const SDL_FRect& destination,
        bool flipHorizontal = false
    ) const;


    // 색상을 입혀서 렌더링
    // 돌 선택 효과 등에 사용
    void RenderTinted(
        SDL_Renderer* renderer,
        const SDL_FRect& source,
        const SDL_FRect& destination,
        Uint8 red,
        Uint8 green,
        Uint8 blue,
        Uint8 alpha = 255
    ) const;


    void Unload();

    bool IsLoaded() const;


private:
    SDL_Texture* texture;
};