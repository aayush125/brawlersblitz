#pragma once
#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include "Entity.hpp"
#include "ResourceManager.hpp"

class Spritesheet {
    private:
        SDL_Rect m_currentFrame{};
        SDL_Texture* m_spritesheet_image = nullptr;
        Entity selected_sprite_entity;
        ResourceManager* m_managerref = nullptr;
        SDL_Rect m_spriteDimensions{};
        bool m_shouldFlip;
        bool m_loop;
        int m_animationSize{};
        bool m_finishedPlaying = false;

        // Animation attributes
        std::vector<int> m_animationFrames;
        float m_animationDelay{};
        int m_currentAnimFrame{};
        float m_animationTimer{};

    public:
        bool m_playOnce = false;
        
        Spritesheet();
        Spritesheet(ResourceManager* manager, const char* path, int row, int column, int animationFps, std::vector<int> animationFrames, bool loop);

        void select_sprite(int x, int y);
        void draw_selected_sprite(SDL_Rect& position);
        void update(float deltaTime);
        void playAnim(SDL_Rect& p_position, bool p_flipFlag);

        const SDL_Rect& getSpriteDim() const;
        const int getCurrentFrameNum() const;
        const bool finishedPlaying() const;
};