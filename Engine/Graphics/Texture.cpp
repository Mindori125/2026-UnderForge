#include "Texture.h"

#include <SDL3_image/SDL_image.h>


Texture::Texture()
    : texture(nullptr)
{
}


Texture::~Texture()
{
    Unload();
}


bool Texture::Load(
    SDL_Renderer* renderer,
    const std::string& filePath
)
{
    Unload();


    texture =
        IMG_LoadTexture(
            renderer,
            filePath.c_str()
        );


    if (texture == nullptr)
    {
        SDL_Log(
            "Failed to load texture: %s - %s",
            filePath.c_str(),
            SDL_GetError()
        );

        return false;
    }


    SDL_SetTextureBlendMode(
        texture,
        SDL_BLENDMODE_BLEND
    );


    return true;
}


void Texture::Render(
    SDL_Renderer* renderer,
    const SDL_FRect& destination
) const
{
    if (texture == nullptr)
    {
        return;
    }


    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        &destination
    );
}


void Texture::Render(
    SDL_Renderer* renderer,
    const SDL_FRect& source,
    const SDL_FRect& destination,
    bool flipHorizontal
) const
{
    if (texture == nullptr)
    {
        return;
    }


    SDL_FlipMode flip =
        SDL_FLIP_NONE;


    if (flipHorizontal)
    {
        flip =
            SDL_FLIP_HORIZONTAL;
    }


    SDL_RenderTextureRotated(
        renderer,
        texture,
        &source,
        &destination,
        0.0,
        nullptr,
        flip
    );
}


void Texture::RenderTinted(
    SDL_Renderer* renderer,
    const SDL_FRect& destination,
    Uint8 red,
    Uint8 green,
    Uint8 blue,
    Uint8 alpha
) const
{
    if (texture == nullptr)
    {
        return;
    }


    SDL_SetTextureColorMod(
        texture,
        red,
        green,
        blue
    );


    SDL_SetTextureAlphaMod(
        texture,
        alpha
    );


    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        &destination
    );


    // 원래 색상으로 복구
    SDL_SetTextureColorMod(
        texture,
        255,
        255,
        255
    );


    SDL_SetTextureAlphaMod(
        texture,
        255
    );
}


void Texture::RenderTinted(
    SDL_Renderer* renderer,
    const SDL_FRect& source,
    const SDL_FRect& destination,
    Uint8 red,
    Uint8 green,
    Uint8 blue,
    Uint8 alpha
) const
{
    if (texture == nullptr)
    {
        return;
    }


    SDL_SetTextureColorMod(
        texture,
        red,
        green,
        blue
    );


    SDL_SetTextureAlphaMod(
        texture,
        alpha
    );


    SDL_RenderTexture(
        renderer,
        texture,
        &source,
        &destination
    );


    // 원래 색상으로 복구
    SDL_SetTextureColorMod(
        texture,
        255,
        255,
        255
    );


    SDL_SetTextureAlphaMod(
        texture,
        255
    );
}


void Texture::Unload()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(
            texture
        );

        texture =
            nullptr;
    }
}


bool Texture::IsLoaded() const
{
    return texture != nullptr;
}