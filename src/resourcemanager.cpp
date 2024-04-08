#include "ResourceManager.hpp"

ResourceManager::ResourceManager(RenderWindow& p_window) : m_windowref(p_window) {}

SDL_Texture* ResourceManager::loadTex(const char *path) {
    return m_windowref.loadTexture(path);
}

void ResourceManager::renderTex(SDL_Texture* p_texture, SDL_Rect& srcrect, SDL_Rect& dstrect, bool shouldFlip) {
    m_windowref.render(p_texture, srcrect, dstrect, shouldFlip);
}

void ResourceManager::renderBackground(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect) {
    m_windowref.renderBackground(p_texture, p_srcrect, p_dstrect);
}

const std::string& ResourceManager::getBasePathFromWindow() {
    return m_windowref.getBasePath();
}