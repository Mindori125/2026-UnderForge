#pragma once

#include <SDL3/SDL.h>
#include <string>

class Texture
{
public:
    Texture();
    ~Texture();

    bool Load(SDL_Renderer* renderer, const std::string& filePath);
    void Render(SDL_Renderer* renderer, const SDL_FRect& destination);
    void Unload();

    bool IsLoaded() const;

private:
    SDL_Texture* texture;
};