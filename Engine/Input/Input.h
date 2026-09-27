#pragma once

#include <SDL3/SDL.h>

enum class Key
{
    W,
    A,
    S,
    D
};

class Input
{
public:
    static bool IsKeyDown(Key key);

private:
    static SDL_Scancode ToSDLScancode(Key key);
};