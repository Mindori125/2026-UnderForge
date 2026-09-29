#include "Input.h"


bool Input::IsKeyDown(Key key)
{
    const bool* keyboard =
        SDL_GetKeyboardState(nullptr);

    // Shift는 왼쪽 / 오른쪽 둘 다 허용
    if (key == Key::Shift)
    {
        return
            keyboard[SDL_SCANCODE_LSHIFT] ||
            keyboard[SDL_SCANCODE_RSHIFT];
    }

    SDL_Scancode scancode =
        ToSDLScancode(key);

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