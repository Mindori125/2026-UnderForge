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

    texture = IMG_LoadTexture(renderer, filePath.c_str());

    if (texture == nullptr)
    {
        SDL_Log(
            "Failed to load texture: %s - %s",
            filePath.c_str(),
            SDL_GetError()
        );

        return false;
    }

    return true;
}

void Texture::Render(
    SDL_Renderer* renderer,
    const SDL_FRect& destination
)
{
    if (texture == nullptr)
        return;

    SDL_RenderTexture(
        renderer,
        texture,
        nullptr,
        &destination
    );
}

void Texture::Unload()
{
    if (texture != nullptr)
    {
        SDL_DestroyTexture(texture);
        texture = nullptr;
    }
}

bool Texture::IsLoaded() const
{
    return texture != nullptr;
}