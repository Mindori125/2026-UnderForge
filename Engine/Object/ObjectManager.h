#pragma once

#include <vector>
#include "GameObject.h"

class Camera;

class ObjectManager
{
public:
    ObjectManager();
    ~ObjectManager();

    void AddObject(GameObject* object);

    void Update(float deltaTime);

    void Render(
        SDL_Renderer* renderer,
        const Camera& camera
    );

    const std::vector<GameObject*>& GetObjects() const;

private:
    std::vector<GameObject*> objects;
};