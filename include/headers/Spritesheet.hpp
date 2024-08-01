#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "Entity.hpp"
#include "ResourceManager.hpp"

class Spritesheet
{
private:
    SDL_Rect mCurrentFrame{};
    SDL_Texture *mSpritesheetImage = nullptr;
    Entity mSelectedSpriteEntity;
    ResourceManager &mManager;
    SDL_Rect mSpriteDimensions{};
    bool mShouldFlip;
    bool mLoop;
    bool mStopOnLastFrame = false;
    int mAnimationSize{};
    bool mFinishedPlaying = false;

    // Animation attributes
    std::vector<int> mAnimationFrames;
    float mAnimationDelay{};
    int mCurrentAnimFrame{};
    float mAnimationTimer{};

public:
    Spritesheet(ResourceManager &manager, const char *path, int row, int column, int animationFps, std::vector<int> animationFrames, bool loop, bool stopOnLastFrame);

    void select_sprite(int x, int y);
    void draw_selected_sprite(SDL_Rect &position);
    void update(float deltaTime);
    void play_spritesheet(SDL_Rect &p_position, bool p_flipFlag);

    const SDL_Rect &get_sprite_dims() const;
    const int get_current_frame_number() const;
    const bool finished_playing() const;
};