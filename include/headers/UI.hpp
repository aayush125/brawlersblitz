#pragma once

#include <SDL2/SDL.h>

#include "ResourceManager.hpp"
#include "Utils.hpp"

class UI
{
public:
    UI(ResourceManager &pManager);

    void init_ingame_ui();
    void render_ingame_ui();

    void update(float pPlayerOneHealth, float pPlayerTwoHealth, int pPlayerOneScore, int pPlayerTwoScore, int pWindowHeight, int pWindowWidth);

private:
    ResourceManager &mResManager;
    void load_score_digits();
    void update_score_digits(int p_window_height, int p_window_width, int p_player_one_score, int p_player_two_score);
    void render_score_digits();
    SDL_Texture *mScoreDigits[4] = {nullptr, nullptr, nullptr, nullptr};
    SDL_Texture *mPlayerOneScore = nullptr;
    SDL_Texture *mPlayerTwoScore = nullptr;
    SDL_Rect mPlayerOneScoreRect = {0, 25, SCORE_RENDER_HEIGHT, SCORE_RENDER_WIDTH};
    SDL_Rect mPlayerTwoScoreRect = {0, 25, SCORE_RENDER_HEIGHT, SCORE_RENDER_WIDTH};

    class HealthBar
    {
    public:
        HealthBar();
        void load_healthbar(int pPlayerID, ResourceManager &pManager);
        void render(ResourceManager &pManager);
        void update_health_bar(float pPlayerHealth);

    private:
        SDL_Texture *mHealthBarOutline = nullptr; // Renders on top of the fill. Outline render rects are calculated in the render() function.
        SDL_Texture *mHealthBarFill = nullptr;
        SDL_Rect mFillSrcRect{};         //  The "source rect" for the fill.
        SDL_Rect mFillDestRect{};        // The "destination rect" for the fill. Determines how much of the fill to render.
        SDL_Point mOriginalDimensions{}; // Stores width and height for scaling purposes.
    };

    HealthBar mPlayerOneHealthBar;
    HealthBar mPlayerTwoHealthBar;
};