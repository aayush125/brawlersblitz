#pragma once
#include <SDL2/SDL.h>

#define PRIMARY_RES_W 1920.0f
#define PRIMARY_RES_H 1080.0f

#define SPRITE_SCALE_FACTOR 5
#define HEALTHBAR_SCALE_X 6
#define HEALTHBAR_SCALE_Y 8

#define PLAYER_ONE_HEALTHBAR_X 30
#define PLAYER_TWO_HEALTHBAR_X 1236
#define HEALTHBAR_Y 30

#define CHAR_NORMAL_WIDTH 40
#define MARTIAL_ATTACK_WIDTH 80
#define WIZARD_ATTACK_WIDTH 62

#define PLAYER_ONE_INIT_X 10
#define PLAYER_TWO_INIT_X 850

#define WIZARD_SCALE 3

#define SCORE_HEIGHT 10
#define SCORE_WIDTH 7

#define SCORE_RENDER_HEIGHT 50
#define SCORE_RENDER_WIDTH 50

namespace utils
{
    inline float hireTimeInSeconds()
    {
        float t = SDL_GetTicks();
        t *= 0.001f;

        return t;
    }
}

namespace level
{
    enum levels
    {
        MENU,
        LEVELONE
    };
}

class Vector2f
{
public:
    float x{};
    float y{};

    Vector2f(float, float);
};