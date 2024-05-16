#include <SDL2/SDL.h>

#include "Characters.hpp"

// Physics related functions such as gravity. Really just gravity right now

// Gravity function. Takes a character's reference and continuously decrements it's y-value in a timely fashion as long as it's greater than the ground coordinate

class Physics {
    private:
        int m_groundCoords{};
    
    public:
        Physics();
        Physics(int p_groundCoords);

        void set_ground_coords(int pGroundCoords);

        void gravity(CharacterBase& p_character, float p_deltaTime);
        bool collides(const SDL_Rect& p_rectOne, const SDL_Rect& p_rectTwo, int p_charWidthOne, int p_charWidthTwo);
};