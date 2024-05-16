#pragma once

#include <SDL2/SDL.h>

#include "ResourceManager.hpp"

class UI {
    public:
        UI(ResourceManager& pManager);

        void init_ingame_ui();
        void render_ingame_ui();

    private:
        ResourceManager& mResManager;

        class HealthBar {
            public:
                HealthBar();
                void load_healthbar(int pPlayerID, ResourceManager& pManager);
                void render(ResourceManager& pManager);

            private:

                SDL_Texture* mHealthBarOutline = nullptr; // Renders on top of the fill
                SDL_Texture* mHealthBarFill = nullptr;
                SDL_Rect mHealthBarRenderRect{}; // Determines how much of the fill to render
                SDL_Rect mFillDestRect{};
                SDL_Point mOriginalDimensions{};
        };

        HealthBar mPlayerOneHealthBar;
        HealthBar mPlayerTwoHealthBar;
};