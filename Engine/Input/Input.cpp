#include "Input.h"

bool Input::IsKeyDown(Key key)
{
    const bool* keyboard = SDL_GetKeyboardState(nullptr);

    SDL_Scancode scancode = ToSDLScancode(key);

    return keyboard[scancode];
}

SDL_Scancode Input::ToSDLScancode(Key key)
{
    switch (key)
    {
        case Key::W:
            return SDL_SCANCODE_W;

        case Key::A:
            return SDL_SCANCODE_A;

        case Key::S:
            return SDL_SCANCODE_S;

        case Key::D:
            return SDL_SCANCODE_D;

        default:
            return SDL_SCANCODE_UNKNOWN;
    }
}