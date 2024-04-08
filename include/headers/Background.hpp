#pragma once
#include <SDL2/SDL.h>
#include <vector>
#include "RenderWindow.hpp"
#include "ResourceManager.hpp"

class Background {
    private:
        std::vector<SDL_Rect> m_collisionBoxes;
        SDL_Texture* m_background;
        ResourceManager& m_resManager;

        void loadBackground(int p_level);
    
    public:
        Background(int p_level, ResourceManager& p_manager);

        enum collionBoxes {
            GROUND,
            PLATFORMONE,
            PLATFORMTWO,
            PLATFORMTHREE,
            PLATFORMFOUR
        };

        void render();
        void addCollider(SDL_Rect p_collider);
        SDL_Rect& getCollider(int p_level);
};