#include "Physics.hpp"
#include <iostream>

Physics::Physics(int p_groundCoords) : m_groundCoords{p_groundCoords} {}

void Physics::gravity(CharacterBase& p_character, float p_deltaTime) {
    const float gravity = 1300.0f;

    if (p_character.m_isInAir) {
        p_character.m_verticalVelocity += gravity * p_deltaTime;

        SDL_Rect position = p_character.getCurrentPosition();
        if ((position.y + position.h) >= m_groundCoords) {
            position.y = m_groundCoords - position.h;
            p_character.setPosition(position);
            p_character.m_isInAir = false;
            p_character.m_verticalVelocity = 0.0f;
        } else {
            p_character.setPosition(position);
        }
    }
}

bool Physics::collides(const SDL_Rect& rectOne, const SDL_Rect& rectTwo, int p_charWidthOne, int p_charWidthTwo) {
    // TODO: Find a better way to detect collision than to use an arbitrary value like 34 (average width of the sprites) * SPRITE_SCALE_FACTOR (scaling factor)
    SDL_Rect p_rectOne = {rectOne.x + (rectOne.w / 2 - ((p_charWidthOne * SPRITE_SCALE_FACTOR) / 2)), rectOne.y, p_charWidthOne * SPRITE_SCALE_FACTOR, rectOne.h};
    SDL_Rect p_rectTwo = {rectTwo.x + ((rectTwo.w / 2 - ((p_charWidthTwo * SPRITE_SCALE_FACTOR) / 2))), rectTwo.y, p_charWidthTwo * SPRITE_SCALE_FACTOR, rectTwo.h};
    
    if ((p_rectOne.x + p_rectOne.w) >= (p_rectTwo.x) && (p_rectOne.x) <= (p_rectTwo.x + p_rectTwo.w)) {
        std::cout << "First is true\n";
        std::cout << "p_rectOne right edge: " << p_rectOne.x + p_rectOne.w << '\n';
        std::cout << "p_rectTwo.x: " << p_rectTwo.x << '\n';
        return true; // Left to right collision
    } else if ((p_rectTwo.x + p_rectTwo.w) >= (p_rectOne.x) && (p_rectTwo.x) <= (p_rectOne.x + p_rectOne.w)) {
        std::cout << "Second is true\n";
        return true; // Right to left collision
    }
    // } else if (p_rectOne.y + p_rectOne.h >= p_rectTwo.y) {
    //     return true; // Top to bottom collision
    // } else if (p_rectTwo.y + p_rectTwo.h >= p_rectOne.y) {
    //     return true; // Bottom to top collision
    // }

    return false;
}