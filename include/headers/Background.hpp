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
        int mScene{};
    
    public:
        Background(ResourceManager& p_manager);

        Background(int p_level, ResourceManager& p_manager);

        enum collionBoxes {
            GROUND,
            PLATFORMONE,
            PLATFORMTWO,
            PLATFORMTHREE,
            PLATFORMFOUR
        };

        void init(int pScene);
        void set_scene(int pLevel);
        void load();
        void render();
        void addCollider(SDL_Rect p_collider);
        SDL_Rect& getCollider(int p_level);
};