#include "Spritesheet.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

Spritesheet::Spritesheet(ResourceManager &manager, const char *path, int row, int column, int animationFps, std::vector<int> animationFrames, bool loop, bool stopOnLastFrame) : mManager{manager}
{

    mSpritesheetImage = mManager.load_tex(path);

    mAnimationFrames = animationFrames;
    mAnimationSize = animationFrames.size();
    mAnimationDelay = 1.0f / animationFps;
    mCurrentAnimFrame = 0;
    mAnimationTimer = 0.0f;
    mShouldFlip = false;
    mLoop = loop;
    mStopOnLastFrame = stopOnLastFrame;

    SDL_QueryTexture(mSpritesheetImage, NULL, NULL, &mCurrentFrame.w, &mCurrentFrame.h);
    mCurrentFrame.w /= column;
    mCurrentFrame.h /= row;

    mSpriteDimensions.w = mCurrentFrame.w;
    mSpriteDimensions.h = mCurrentFrame.h;
}

void Spritesheet::select_sprite(int x, int y)
{
    mCurrentFrame.x = x * mCurrentFrame.w;
    mCurrentFrame.y = y * mCurrentFrame.h;
}

void Spritesheet::draw_selected_sprite(SDL_Rect &position)
{
    mManager.render_tex(mSpritesheetImage, mCurrentFrame, position, mShouldFlip);
}

void Spritesheet::update(float deltaTime)
{
    if (!mLoop && mCurrentAnimFrame == mAnimationSize - 1)
    {
        mFinishedPlaying = true;
        if (!mStopOnLastFrame)
            mCurrentAnimFrame = 0;
        return;
    }

    mFinishedPlaying = false;

    mAnimationTimer += deltaTime;

    if (mAnimationTimer >= mAnimationDelay)
    {
        mCurrentAnimFrame = (mCurrentAnimFrame + 1) % mAnimationFrames.size();
        mAnimationTimer = 0.0f;
    }
}

void Spritesheet::play_spritesheet(SDL_Rect &p_position, bool p_flipFlag)
{
    mShouldFlip = p_flipFlag;

    select_sprite(mAnimationFrames[mCurrentAnimFrame], 0);
    draw_selected_sprite(p_position);
}

const SDL_Rect &Spritesheet::get_sprite_dims() const
{
    return mSpriteDimensions;
}

const int Spritesheet::get_current_frame_number() const
{
    return mCurrentAnimFrame;
}

const bool Spritesheet::finished_playing() const
{
    return mFinishedPlaying;
}