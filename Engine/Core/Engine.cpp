#include "Engine.h"
#include <emscripten.h>

static Engine* g_engine = nullptr;

static void MainLoopCallback()
{
    if (g_engine != nullptr)
    {
        g_engine->GameLoop();
    }
}

Engine::Engine()
    : window(nullptr),
      renderer(nullptr),
      screenWidth(0),
      screenHeight(0),
      running(false),
      lastTime(0),
      player(nullptr)
{
}

Engine::~Engine()
{
    Shutdown();
}

bool Engine::Initialize(const char* title, int width, int height)
{
    screenWidth = width;
    screenHeight = height;

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    if (!SDL_CreateWindowAndRenderer(
            title,
            width,
            height,
            0,
            &window,
            &renderer))
    {
        SDL_Log("Window creation failed: %s", SDL_GetError());
        SDL_Quit();
        return false;
    }

    player = new Player(
    width / 2.0f - 25.0f,
    height / 2.0f - 25.0f,
    50.0f,
    50.0f
    );
    
    player->SetScreenSize(width, height);

    running = true;
    lastTime = SDL_GetTicks();

    return true;
}

void Engine::ProcessInput()
{
    SDL_Event event;

    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_EVENT_QUIT)
        {
            running = false;
        }
    }
}

void Engine::Update(float deltaTime)
{
    if (player != nullptr)
    {
        player->Update(deltaTime);
    }
}

void Engine::Render()
{
    SDL_SetRenderDrawColor(renderer, 25, 25, 35, 255);
    SDL_RenderClear(renderer);

    if (player != nullptr)
    {
        player->Render(renderer);
    }

    SDL_RenderPresent(renderer);
}

void Engine::GameLoop()
{
    if (!running)
    {
        emscripten_cancel_main_loop();
        return;
    }

    Uint64 currentTime = SDL_GetTicks();

    float deltaTime =
        static_cast<float>(currentTime - lastTime) / 1000.0f;

    lastTime = currentTime;

    ProcessInput();
    Update(deltaTime);
    Render();
}

void Engine::Run()
{
    g_engine = this;

    emscripten_set_main_loop(
        MainLoopCallback,
        0,
        true
    );
}

void Engine::Shutdown()
{
    if (player != nullptr)
    {
        delete player;
        player = nullptr;
    }

    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}