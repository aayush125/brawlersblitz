#include "Background.hpp"
#include <vector>
#include <string>

Background::Background(ResourceManager& p_manager) : m_resManager{p_manager} {}

Background::Background(int p_level, ResourceManager &p_manager) : m_resManager(p_manager) {
}

void Background::set_scene(int pScene) {
    if (pScene == mScene) {
        return;
    }

    mScene = pScene;
    init(pScene);
}

void Background::init(int pScene) {
    mScene = pScene;
    load();
}

void Background::load() {
    std::string backgroundFile;
    switch (mScene)
    {
    case 0:
        backgroundFile = "menu.png";
        break;
    case 1:
        backgroundFile = "background1.png";
        addCollider(SDL_Rect{0, 992, 1920, 94});
        break;
    case 2:
        backgroundFile = "background2.png";
        break;
    case 3:
        backgroundFile = "background3.png";
        break;
    case 4:
        backgroundFile = "background4.png";
        break;
    default:
        backgroundFile = "background1.png";
        break;
    }

    m_background = m_resManager.load_tex((m_resManager.get_base_path_from_window() + "../assets/backgrounds/" + backgroundFile).c_str());
}

void Background::render()
{
    m_resManager.render_bg(m_background, nullptr, nullptr);
}

void Background::addCollider(SDL_Rect p_collider) {
    m_collisionBoxes.push_back(p_collider);
}

SDL_Rect& Background::getCollider(int p_level) {
    return m_collisionBoxes[p_level];
}