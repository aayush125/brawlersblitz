#pragma once
#include <SDL2/SDL.h>
#include <string>

#include "RenderWindow.hpp"

class ResourceManager {
    private:
        RenderWindow& m_windowref;
    
    public:
        ResourceManager(RenderWindow& p_window);

        SDL_Texture* loadTex(const char* path);
        void renderTex(SDL_Texture* p_texture, SDL_Rect&, SDL_Rect&, bool);
        void renderBackground(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect);

        const std::string& getBasePathFromWindow();
};