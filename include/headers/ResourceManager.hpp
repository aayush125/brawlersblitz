#pragma once
#include <SDL2/SDL.h>
#include <string>

#include "RenderWindow.hpp"

class ResourceManager
{
private:
    RenderWindow &m_windowref;

public:
    ResourceManager(RenderWindow &p_window);

    SDL_Texture *load_tex(const char *path);
    void render_tex(SDL_Texture *p_texture, SDL_Rect &, SDL_Rect &, bool);
    void render_ui(SDL_Texture *p_texture, SDL_Rect *p_srcrect, SDL_Rect *p_dstrect);
    void render_bg(SDL_Texture *p_texture, SDL_Rect *p_srcrect, SDL_Rect *p_dstrect);

    const std::string &get_base_path_from_window();
};