#include "Engine/Core/Engine.h"

int main()
{
    Engine engine;

    if (!engine.Initialize("2026 Game Project", 800, 600))
    {
        return 1;
    }

    engine.Run();

    return 0;
}