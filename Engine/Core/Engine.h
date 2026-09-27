#pragma once

#include <SDL3/SDL.h>
#include "../Scene/SceneManager.h"

class Engine
{
public:
    Engine();
    ~Engine();

    bool Initialize(const char* title, int width, int height);
    void Run();
    void GameLoop();
    void Shutdown();

private:
    void ProcessInput();
    void Update(float deltaTime);
    void Render();

private:
    SDL_Window* window;
    SDL_Renderer* renderer;

    int screenWidth;
    int screenHeight;

    bool running;
    Uint64 lastTime;
    SceneManager* sceneManager;
};