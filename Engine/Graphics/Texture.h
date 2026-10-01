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


    void Render(
        SDL_Renderer* renderer,
        const SDL_FRect& destination
    ) const;


    void Render(
        SDL_Renderer* renderer,
        const SDL_FRect& source,
        const SDL_FRect& destination,
        bool flipHorizontal = false
    ) const;


    // 전체 이미지를 색상 변경해서 렌더링
    void RenderTinted(
        SDL_Renderer* renderer,
        const SDL_FRect& destination,
        Uint8 red,
        Uint8 green,
        Uint8 blue,
        Uint8 alpha = 255
    ) const;


    // 이미지 일부를 색상 변경해서 렌더링
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