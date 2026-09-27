#include "HealthBar.h"

HealthBar::HealthBar(
    float x,
    float y,
    float width,
    float height
)
    : x(x),
      y(y),
      width(width),
      height(height)
{
}


void HealthBar::Render(
    SDL_Renderer* renderer,
    int currentHealth,
    int maxHealth
)
{
    if (maxHealth <= 0)
        return;

    float healthPercent =
        static_cast<float>(currentHealth) /
        static_cast<float>(maxHealth);

    // 0 ~ 1 범위로 제한
    if (healthPercent < 0.0f)
        healthPercent = 0.0f;

    if (healthPercent > 1.0f)
        healthPercent = 1.0f;


    // =========================
    // 체력바 테두리
    // =========================

    SDL_FRect borderRect = {
        x - 3.0f,
        y - 3.0f,
        width + 6.0f,
        height + 6.0f
    };

    SDL_SetRenderDrawColor(
        renderer,
        10,
        10,
        10,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &borderRect
    );


    // =========================
    // 체력바 빈 배경
    // =========================

    SDL_FRect backgroundRect = {
        x,
        y,
        width,
        height
    };

    SDL_SetRenderDrawColor(
        renderer,
        60,
        20,
        20,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &backgroundRect
    );


    // =========================
    // 현재 체력
    // =========================

    SDL_FRect healthRect = {
        x,
        y,
        width * healthPercent,
        height
    };

    SDL_SetRenderDrawColor(
        renderer,
        220,
        40,
        40,
        255
    );

    SDL_RenderFillRect(
        renderer,
        &healthRect
    );
}