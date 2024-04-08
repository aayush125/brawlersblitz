#pragma once
#include <SDL2/SDL.h>

#define PRIMARY_RES_W 1920.0f
#define PRIMARY_RES_H 1080.0f

#define SPRITE_SCALE_FACTOR 5

#define CHAR_NORMAL_WIDTH 34
#define CHAR_ATTACK_WIDTH 78

namespace utils {
    inline float hireTimeInSeconds() {
        float t = SDL_GetTicks();
        t *= 0.001f;

        return t;
    }
}

namespace level {
    enum levels {
        MENU,
        LEVELONE
    };
}

class Vector2f {
    public:
        float x{};
        float y{};

        Vector2f(float, float);
};