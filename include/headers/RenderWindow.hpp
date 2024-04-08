#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <string>

#include "Entity.hpp"
#include "Utils.hpp"

class RenderWindow {
    private:
        SDL_Window* m_window;
        SDL_Renderer* m_renderer;
        int m_windowHeight{};
        int m_windowWidth{};
        SDL_Rect m_viewport;
        float m_scaleX{};
        float m_scaleY{};
        
        std::string m_basePath;
    public:
        RenderWindow(const char* p_title, int p_w, int p_h);
        SDL_Texture* loadTexture(const char* p_filePath);
        void cleanUp();
        void clear();
        void render(Entity& p_entity);
        void render(SDL_Texture* p_texture, SDL_Rect& p_srcrect, SDL_Rect& p_dstrect, bool p_shouldFlip);
        void renderBackground(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect);
        void display();
        void fullscreen();
        
        SDL_Renderer* getRenderer();
        const std::string& getBasePath();
        const int getWindowHeight() const;
        const int getWindowWidth() const;
        const SDL_Rect GetWindowRect() const;
        void recalculateScalingFactors();
        const Vector2f getScalingFactors() const;
};