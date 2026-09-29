#include "Player.h"

#include <cmath>

#include "../Engine/Input/Input.h"
#include "../Engine/Graphics/Camera.h"


Player::Player(
    float x,
    float y,
    float width,
    float height
)
    : GameObject(x, y, width, height),

      // =========================
      // Movement
      // =========================

      walkSpeed(225.0f),
      runMultiplier(1.5f),

      moveIntentX(0.0f),
      moveIntentY(0.0f),

      isMoving(false),
      isRunning(false),
      facingLeft(false),

      // =========================
      // Health
      // =========================

      health(100),
      maxHealth(100),

      // =========================
      // Stamina
      // =========================

      stamina(10.0f),
      maxStamina(10.0f),

      // 초당 1 감소
      // 최대 스태미나 10 = 최대 10초 달리기
      staminaDrainRate(1.0f),

      // 초당 2 회복
      // 완전 소진 기준 약 5초
      staminaRegenRate(0.7f),

      // 달리기 종료 후 1초 뒤 회복
      staminaRegenDelay(1.0f),
      staminaRegenTimer(0.0f),

      // =========================
      // Rendering
      // =========================

      renderWidth(192.0f),
      renderHeight(192.0f),

      // 이전 요청대로
      // Hitbox를 올리는 것이 아니라
      // 캐릭터 이미지를 아래로 이동
      spriteOffsetY(70.0f)
{
    // Idle
    idleAnimation.Setup(
        128,
        128,
        10,
        0.12f
    );


    // Walk
    walkAnimation.Setup(
        128,
        128,
        10,
        0.08f
    );


    // Run
    runAnimation.Setup(
        128,
        128,
        10,
        0.06f
    );
}


bool Player::LoadTextures(SDL_Renderer* renderer)
{
    bool idleLoaded =
        idleTexture.Load(
            renderer,
            "Assets/Textures/Player/player_idle.png"
        );


    bool walkLoaded =
        walkTexture.Load(
            renderer,
            "Assets/Textures/Player/player_walk.png"
        );


    bool runLoaded =
        runTexture.Load(
            renderer,
            "Assets/Textures/Player/player_run.png"
        );


    if (!idleLoaded)
    {
        SDL_Log(
            "Failed to load player_idle.png"
        );
    }


    if (!walkLoaded)
    {
        SDL_Log(
            "Failed to load player_walk.png"
        );
    }


    if (!runLoaded)
    {
        SDL_Log(
            "Failed to load player_run.png"
        );
    }


    return
        idleLoaded &&
        walkLoaded &&
        runLoaded;
}


void Player::Update(float deltaTime)
{
    moveIntentX = 0.0f;
    moveIntentY = 0.0f;

    isMoving = false;


    // =========================
    // Movement Input
    // =========================

    if (Input::IsKeyDown(Key::W))
    {
        moveIntentY -= 1.0f;
        isMoving = true;
    }


    if (Input::IsKeyDown(Key::S))
    {
        moveIntentY += 1.0f;
        isMoving = true;
    }


    if (Input::IsKeyDown(Key::A))
    {
        moveIntentX -= 1.0f;

        isMoving = true;

        // 왼쪽 방향 기억
        facingLeft = true;
    }


    if (Input::IsKeyDown(Key::D))
    {
        moveIntentX += 1.0f;

        isMoving = true;

        // 오른쪽 방향 기억
        facingLeft = false;
    }


    // =========================
    // Diagonal normalization
    // =========================

    if (isMoving)
    {
        float length =
            std::sqrt(
                moveIntentX * moveIntentX +
                moveIntentY * moveIntentY
            );


        if (length > 0.0f)
        {
            moveIntentX /= length;
            moveIntentY /= length;
        }
    }


    // =========================
    // Running
    // =========================

    bool shiftPressed =
        Input::IsKeyDown(Key::Shift);


    bool wantsToRun =
        isMoving &&
        shiftPressed;


    isRunning =
        wantsToRun &&
        stamina > 0.0f;


    // =========================
    // Stamina Drain
    // =========================

    if (isRunning)
    {
        stamina -=
            staminaDrainRate *
            deltaTime;


        if (stamina <= 0.0f)
        {
            stamina = 0.0f;

            // 스태미나가 완전히 떨어지면
            // 즉시 걷기로 전환
            isRunning = false;
        }


        // 달리는 동안에는
        // 회복 대기시간 초기화
        staminaRegenTimer = 0.0f;
    }
    else
    {
        // =========================
        // Stamina Regeneration
        // =========================

        // Shift를 누른 채 달리려고 하는 상황에서는
        // 스태미나 회복 금지.
        //
        // Shift를 떼거나 이동을 멈춰야
        // 회복 대기시간이 시작됨.

        if (!wantsToRun)
        {
            if (stamina < maxStamina)
            {
                staminaRegenTimer +=
                    deltaTime;


                // 1초 기다린 뒤 회복
                if (
                    staminaRegenTimer >=
                    staminaRegenDelay
                )
                {
                    stamina +=
                        staminaRegenRate *
                        deltaTime;


                    if (stamina >= maxStamina)
                    {
                        stamina =
                            maxStamina;

                        staminaRegenTimer =
                            0.0f;
                    }
                }
            }
            else
            {
                staminaRegenTimer =
                    0.0f;
            }
        }
        else
        {
            staminaRegenTimer =
                0.0f;
        }
    }


    // =========================
    // Animation
    // =========================

    if (isRunning)
    {
        runAnimation.Update(
            deltaTime
        );

        walkAnimation.Reset();
        idleAnimation.Reset();
    }
    else if (isMoving)
    {
        walkAnimation.Update(
            deltaTime
        );

        runAnimation.Reset();
        idleAnimation.Reset();
    }
    else
    {
        idleAnimation.Update(
            deltaTime
        );

        walkAnimation.Reset();
        runAnimation.Reset();
    }
}


void Player::Render(
    SDL_Renderer* renderer,
    const Camera& camera
)
{
    SDL_FRect screenPosition =
        camera.WorldToScreen(rect);


    SDL_FRect renderRect;

    renderRect.w =
        renderWidth;

    renderRect.h =
        renderHeight;


    // 캐릭터 중심 정렬
    renderRect.x =
        screenPosition.x +
        (rect.w / 2.0f) -
        (renderWidth / 2.0f);


    // =========================
    // 중요
    //
    // Hitbox를 위로 올리지 않고
    // 캐릭터 이미지 자체를
    // 24px 아래로 내림
    // =========================

    renderRect.y =
        screenPosition.y +
        rect.h -
        renderHeight +
        spriteOffsetY;


    // =========================
    // Run
    // =========================

    if (isRunning)
    {
        SDL_FRect sourceRect =
            runAnimation.GetCurrentFrame();


        runTexture.Render(
            renderer,
            sourceRect,
            renderRect,
            facingLeft
        );

        return;
    }


    // =========================
    // Walk
    // =========================

    if (isMoving)
    {
        SDL_FRect sourceRect =
            walkAnimation.GetCurrentFrame();


        walkTexture.Render(
            renderer,
            sourceRect,
            renderRect,
            facingLeft
        );

        return;
    }


    // =========================
    // Idle
    // =========================

    SDL_FRect sourceRect =
        idleAnimation.GetCurrentFrame();


    idleTexture.Render(
        renderer,
        sourceRect,
        renderRect,
        facingLeft
    );
}


SDL_FRect Player::GetHitbox() const
{
    SDL_FRect hitbox;


    hitbox.w =
        28.0f;

    hitbox.h =
        18.0f;


    hitbox.x =
        rect.x +
        (rect.w - hitbox.w) /
        2.0f;


    hitbox.y =
        rect.y +
        rect.h -
        hitbox.h;


    return hitbox;
}


float Player::GetMoveDeltaX(
    float deltaTime
) const
{
    float currentSpeed =
        walkSpeed;


    if (isRunning)
    {
        currentSpeed *=
            runMultiplier;
    }


    return
        moveIntentX *
        currentSpeed *
        deltaTime;
}


float Player::GetMoveDeltaY(
    float deltaTime
) const
{
    float currentSpeed =
        walkSpeed;


    if (isRunning)
    {
        currentSpeed *=
            runMultiplier;
    }


    return
        moveIntentY *
        currentSpeed *
        deltaTime;
}


void Player::Move(
    float dx,
    float dy
)
{
    rect.x += dx;
    rect.y += dy;
}


void Player::TakeDamage(int damage)
{
    health -= damage;


    if (health < 0)
    {
        health = 0;
    }
}


int Player::GetHealth() const
{
    return health;
}


int Player::GetMaxHealth() const
{
    return maxHealth;
}


float Player::GetStamina() const
{
    return stamina;
}


float Player::GetMaxStamina() const
{
    return maxStamina;
}


bool Player::IsRunning() const
{
    return isRunning;
}