#pragma once

#include <SDL3/SDL.h>

class Collision
{
public:
    static bool CheckAABB(
        const SDL_FRect& a,
        const SDL_FRect& b
    );
};