#include "GameScene.h"
#include "Player.h"
#include "Enemy.h"

#include "../Engine/Physics/Collision.h"


GameScene::GameScene()
    : objectManager(nullptr),
      player(nullptr),
      enemy(nullptr),
      camera(nullptr)
{
}


GameScene::~GameScene()
{
    // ObjectManager가 Player와 Enemy를 삭제함
    if (objectManager != nullptr)
    {
        delete objectManager;
        objectManager = nullptr;
    }

    // 따라서 Player와 Enemy는 여기서 delete 하지 않음
    player = nullptr;
    enemy = nullptr;

    // Camera 정리
    if (camera != nullptr)
    {
        delete camera;
        camera = nullptr;
    }
}


bool GameScene::Initialize(SDL_Renderer* renderer)
{
    // 게임 오브젝트 관리자 생성
    objectManager = new ObjectManager();

    // 800x600 크기의 카메라 생성
    camera = new Camera(
        800.0f,
        600.0f
    );


    // Player 생성
    player = new Player(
        375.0f,
        275.0f,
        50.0f,
        50.0f
    );

    player->SetScreenSize(
        800,
        600
    );

    // Player 이미지 불러오기
    player->LoadTexture(
        renderer,
        "Assets/Textures/player.png"
    );

    // ObjectManager에 Player 등록
    objectManager->AddObject(player);


    // Enemy 생성
    enemy = new Enemy(
        600.0f,
        200.0f,
        50.0f,
        50.0f
    );

    // ObjectManager에 Enemy 등록
    objectManager->AddObject(enemy);


    return true;
}


void GameScene::Update(float deltaTime)
{
    if (objectManager == nullptr)
        return;


    // 모든 게임 오브젝트 업데이트
    objectManager->Update(deltaTime);


    // 카메라가 Player의 중심을 따라감
    if (camera != nullptr && player != nullptr)
    {
        camera->Follow(
            player->GetX() +
                player->GetBounds().w / 2.0f,

            player->GetY() +
                player->GetBounds().h / 2.0f
        );
    }


    // Player ↔ Enemy 충돌 검사
    if (player != nullptr && enemy != nullptr)
    {
        bool isColliding = Collision::CheckAABB(
            player->GetBounds(),
            enemy->GetBounds()
        );

        enemy->SetColliding(isColliding);
    }
}


void GameScene::Render(SDL_Renderer* renderer)
{
    if (objectManager != nullptr &&
        camera != nullptr)
    {
        objectManager->Render(
            renderer,
            *camera
        );
    }
}