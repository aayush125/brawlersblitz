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
        SDL_Texture* load_texture(const char* p_filePath);
        void cleanup();
        void clear();
        void render(Entity& p_entity);
        void render(SDL_Texture* p_texture, SDL_Rect& p_srcrect, SDL_Rect& p_dstrect, bool p_shouldFlip);
        void render_background(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect);
        void render_ui(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect);
        void display();
        void fullscreen();
        
        SDL_Renderer* get_renderer();
        const std::string& get_base_path();
        const int get_window_height() const;
        const int get_window_width() const;
        const SDL_Rect get_window_rect() const;
        void recalculate_scaling_factors();
        const Vector2f get_scaling_factors() const;
};