#include "Engine.h"
#include "../../Game/GameScene.h"

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
      sceneManager(nullptr)
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

    // SDL 초기화
    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log("SDL_Init failed: %s", SDL_GetError());
        return false;
    }

    // Window + Renderer 생성
    if (!SDL_CreateWindowAndRenderer(
            title,
            width,
            height,
            0,
            &window,
            &renderer))
    {
        SDL_Log(
            "Window creation failed: %s",
            SDL_GetError()
        );

        SDL_Quit();
        return false;
    }

    // SceneManager 생성
    sceneManager = new SceneManager();

    // 첫 번째 Scene으로 GameScene 실행
    sceneManager->ChangeScene(
        new GameScene(),
        renderer
    );

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
    if (sceneManager != nullptr)
    {
        sceneManager->Update(deltaTime);
    }
}


void Engine::Render()
{
    // 화면 초기화
    SDL_SetRenderDrawColor(
        renderer,
        25,
        25,
        35,
        255
    );

    SDL_RenderClear(renderer);

    // 현재 Scene 렌더링
    if (sceneManager != nullptr)
    {
        sceneManager->Render(renderer);
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
        static_cast<float>(
            currentTime - lastTime
        ) / 1000.0f;

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
    // Scene 정리
    if (sceneManager != nullptr)
    {
        delete sceneManager;
        sceneManager = nullptr;
    }

    // Renderer 정리
    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(renderer);
        renderer = nullptr;
    }

    // Window 정리
    if (window != nullptr)
    {
        SDL_DestroyWindow(window);
        window = nullptr;
    }

    SDL_Quit();
}