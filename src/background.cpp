#include "Background.hpp"
#include <vector>
#include <string>

Background::Background(int p_level, ResourceManager &p_manager) : m_resManager(p_manager) {
    loadBackground(p_level);
}

void Background::loadBackground(int p_level) {
    std::string backgroundFile;
    switch (p_level)
    {
    case 0:
        backgroundFile = "menu.png";
    case 1:
        backgroundFile = "background1.png";
    case 2:
        backgroundFile = "background2.png";
    case 3:
        backgroundFile = "background3.png";
    case 4:
        backgroundFile = "background4.png";
    default:
        backgroundFile = "background1.png";
    }

    m_background = m_resManager.loadTex((m_resManager.getBasePathFromWindow() + "../assets/backgrounds/" + backgroundFile).c_str());
}

void Background::render()
{
    m_resManager.renderBackground(m_background, nullptr, nullptr);
}

void Background::addCollider(SDL_Rect p_collider) {
    m_collisionBoxes.push_back(p_collider);
}

SDL_Rect& Background::getCollider(int p_level) {
    return m_collisionBoxes[p_level];
}