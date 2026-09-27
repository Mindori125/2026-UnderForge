#pragma once

#include "Scene.h"

class SceneManager
{
public:
    SceneManager();
    ~SceneManager();

    void ChangeScene(Scene* newScene, SDL_Renderer* renderer);

    void Update(float deltaTime);
    void Render(SDL_Renderer* renderer);

private:
    Scene* currentScene;
};