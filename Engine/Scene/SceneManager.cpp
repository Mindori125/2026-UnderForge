#include "SceneManager.h"

SceneManager::SceneManager()
    : currentScene(nullptr)
{
}

SceneManager::~SceneManager()
{
    if (currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }
}

void SceneManager::ChangeScene(
    Scene* newScene,
    SDL_Renderer* renderer
)
{
    if (currentScene != nullptr)
    {
        delete currentScene;
        currentScene = nullptr;
    }

    currentScene = newScene;

    if (currentScene != nullptr)
    {
        currentScene->Initialize(renderer);
    }
}

void SceneManager::Update(float deltaTime)
{
    if (currentScene != nullptr)
    {
        currentScene->Update(deltaTime);
    }
}

void SceneManager::Render(SDL_Renderer* renderer)
{
    if (currentScene != nullptr)
    {
        currentScene->Render(renderer);
    }
}