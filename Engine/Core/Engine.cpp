#include "Engine.h"

#include "../../Game/GameScene.h"

#include <emscripten.h>


static Engine* g_engine =
    nullptr;


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


bool Engine::Initialize(
    const char* title,
    int width,
    int height
)
{
    screenWidth =
        width;

    screenHeight =
        height;


    // =========================
    // SDL 초기화
    // =========================

    if (!SDL_Init(SDL_INIT_VIDEO))
    {
        SDL_Log(
            "SDL_Init failed: %s",
            SDL_GetError()
        );

        return false;
    }


    // =========================
    // Window + Renderer
    // =========================

    if (!SDL_CreateWindowAndRenderer(
            title,
            width,
            height,

            // 창 크기 변경 허용
            SDL_WINDOW_RESIZABLE,

            &window,
            &renderer
        ))
    {
        SDL_Log(
            "Window creation failed: %s",
            SDL_GetError()
        );

        SDL_Quit();

        return false;
    }


    // =========================
    // Logical Resolution
    // =========================
    //
    // 실제 브라우저 화면이
    //
    // 800x600
    // 1280x720
    // 1920x1080
    // 2560x1440
    //
    // 등으로 변경되어도
    //
    // 게임 내부에서는 항상
    //
    // 800x600
    //
    // 좌표계를 사용한다.
    //
    // SDL이 자동으로 확대/축소한다.
    //
    // LETTERBOX를 사용하기 때문에
    // 화면 비율이 달라도
    // 게임 화면이 찌그러지지 않는다.
    // =========================

    if (!SDL_SetRenderLogicalPresentation(
            renderer,
            width,
            height,
            SDL_LOGICAL_PRESENTATION_LETTERBOX
        ))
    {
        SDL_Log(
            "Failed to set logical presentation: %s",
            SDL_GetError()
        );

        return false;
    }


    // =========================
    // SceneManager
    // =========================

    sceneManager =
        new SceneManager();


    sceneManager->ChangeScene(
        new GameScene(),
        renderer
    );


    // =========================
    // Game Start
    // =========================

    running =
        true;


    lastTime =
        SDL_GetTicks();


    return true;
}


void Engine::ProcessInput()
{
    SDL_Event event;


    while (SDL_PollEvent(&event))
    {
        if (
            event.type ==
            SDL_EVENT_QUIT
        )
        {
            running =
                false;
        }
    }
}


void Engine::Update(
    float deltaTime
)
{
    if (sceneManager != nullptr)
    {
        sceneManager->Update(
            deltaTime
        );
    }
}


void Engine::Render()
{
    // =========================
    // 배경
    // =========================

    SDL_SetRenderDrawColor(
        renderer,
        25,
        25,
        35,
        255
    );


    SDL_RenderClear(
        renderer
    );


    // =========================
    // Scene
    // =========================

    if (sceneManager != nullptr)
    {
        sceneManager->Render(
            renderer
        );
    }


    // =========================
    // 화면 출력
    // =========================

    SDL_RenderPresent(
        renderer
    );
}


void Engine::GameLoop()
{
    if (!running)
    {
        emscripten_cancel_main_loop();

        return;
    }


    Uint64 currentTime =
        SDL_GetTicks();


    float deltaTime =
        static_cast<float>(
            currentTime - lastTime
        ) /
        1000.0f;


    lastTime =
        currentTime;


    // 너무 큰 deltaTime 방지
    //
    // 탭 이동 후 다시 돌아왔을 때
    // 플레이어가 순간이동하는 현상 방지
    if (deltaTime > 0.1f)
    {
        deltaTime =
            0.1f;
    }


    ProcessInput();

    Update(
        deltaTime
    );

    Render();
}


void Engine::Run()
{
    g_engine =
        this;


    emscripten_set_main_loop(
        MainLoopCallback,
        0,
        true
    );
}


void Engine::Shutdown()
{
    if (sceneManager != nullptr)
    {
        delete sceneManager;

        sceneManager =
            nullptr;
    }


    if (renderer != nullptr)
    {
        SDL_DestroyRenderer(
            renderer
        );

        renderer =
            nullptr;
    }


    if (window != nullptr)
    {
        SDL_DestroyWindow(
            window
        );

        window =
            nullptr;
    }


    SDL_Quit();
}