#pragma once

#include <SDL2/SDL.h>

#include "ResourceManager.hpp"

class UI {
    public:
        UI(ResourceManager& pManager);

        void init_ingame_ui();
        void render_ingame_ui();

        void update(float pPlayerOneHealth, float pPlayerTwoHealth);

    private:
        ResourceManager& mResManager;

        class HealthBar {
            public:
                HealthBar();
                void load_healthbar(int pPlayerID, ResourceManager& pManager);
                void render(ResourceManager& pManager);
                void update_health_bar(float pPlayerHealth);

            private:

                SDL_Texture* mHealthBarOutline = nullptr; // Renders on top of the fill. Outline render rects are calculated in the render() function.
                SDL_Texture* mHealthBarFill = nullptr;
                SDL_Rect mFillSrcRect{}; //  The "source rect" for the fill.
                SDL_Rect mFillDestRect{}; // The "destination rect" for the fill. Determines how much of the fill to render.
                SDL_Point mOriginalDimensions{}; // Stores width and height for scaling purposes.
        };

        HealthBar mPlayerOneHealthBar;
        HealthBar mPlayerTwoHealthBar;
};