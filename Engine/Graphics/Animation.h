#pragma once

#include <SDL3/SDL.h>

class Animation
{
public:
    Animation();

    void Setup(
        int frameWidth,
        int frameHeight,
        int frameCount,
        float frameTime
    );

    void Update(float deltaTime);

    void Reset();

    SDL_FRect GetCurrentFrame() const;

private:
    int frameWidth;
    int frameHeight;

    int frameCount;
    int currentFrame;

    float frameTime;
    float timer;
};