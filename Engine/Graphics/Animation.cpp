#include "Animation.h"


Animation::Animation()
    : frameWidth(128),
      frameHeight(128),
      frameCount(1),
      currentFrame(0),
      frameTime(0.1f),
      timer(0.0f)
{
}


void Animation::Setup(
    int frameWidth,
    int frameHeight,
    int frameCount,
    float frameTime
)
{
    this->frameWidth = frameWidth;
    this->frameHeight = frameHeight;
    this->frameCount = frameCount;
    this->frameTime = frameTime;

    currentFrame = 0;
    timer = 0.0f;
}


void Animation::Update(float deltaTime)
{
    timer += deltaTime;

    if (timer >= frameTime)
    {
        timer -= frameTime;

        currentFrame++;

        if (currentFrame >= frameCount)
        {
            currentFrame = 0;
        }
    }
}


void Animation::Reset()
{
    currentFrame = 0;
    timer = 0.0f;
}


SDL_FRect Animation::GetCurrentFrame() const
{
    SDL_FRect sourceRect = {
        static_cast<float>(currentFrame * frameWidth),
        0.0f,
        static_cast<float>(frameWidth),
        static_cast<float>(frameHeight)
    };

    return sourceRect;
}