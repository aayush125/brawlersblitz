#include "ResourceManager.hpp"

ResourceManager::ResourceManager(RenderWindow &p_window) : m_windowref(p_window) {}

SDL_Texture *ResourceManager::load_tex(const char *path)
{
    return m_windowref.load_texture(path);
}

void ResourceManager::render_tex(SDL_Texture *p_texture, SDL_Rect &srcrect, SDL_Rect &dstrect, bool shouldFlip)
{
    m_windowref.render(p_texture, srcrect, dstrect, shouldFlip);
}

void ResourceManager::render_ui(SDL_Texture *p_texture, SDL_Rect *p_srcrect, SDL_Rect *p_dstrect)
{
    m_windowref.render_ui(p_texture, p_srcrect, p_dstrect);
}

void ResourceManager::render_bg(SDL_Texture *p_texture, SDL_Rect *p_srcrect, SDL_Rect *p_dstrect)
{
    m_windowref.render_background(p_texture, p_srcrect, p_dstrect);
}

const std::string &ResourceManager::get_base_path_from_window()
{
    return m_windowref.get_base_path();
}