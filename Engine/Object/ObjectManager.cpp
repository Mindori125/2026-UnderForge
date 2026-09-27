#include "ObjectManager.h"
#include "../Graphics/Camera.h"


ObjectManager::ObjectManager()
{
}


ObjectManager::~ObjectManager()
{
    // ObjectManager가 관리하는 모든 GameObject 삭제
    for (GameObject* object : objects)
    {
        delete object;
    }

    objects.clear();
}


void ObjectManager::AddObject(GameObject* object)
{
    objects.push_back(object);
}


void ObjectManager::Update(float deltaTime)
{
    // 모든 오브젝트 업데이트
    for (GameObject* object : objects)
    {
        object->Update(deltaTime);
    }
}


void ObjectManager::Render(
    SDL_Renderer* renderer,
    const Camera& camera
)
{
    // 모든 오브젝트에게 카메라를 전달해서 렌더링
    for (GameObject* object : objects)
    {
        object->Render(
            renderer,
            camera
        );
    }
}


const std::vector<GameObject*>& ObjectManager::GetObjects() const
{
    return objects;
}