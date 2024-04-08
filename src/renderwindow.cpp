#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "RenderWindow.hpp"

RenderWindow::RenderWindow(const char* p_title, int p_w, int p_h) : m_window(NULL), m_renderer(NULL) {
    m_windowHeight = p_h;
    m_windowWidth = p_w;

    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI | SDL_WINDOW_SHOWN);

    m_window = SDL_CreateWindow(p_title, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, p_w, p_h, window_flags);

    if (m_window == NULL) {
        std::cout << "Window failed to initialize. Error:\n" << SDL_GetError() << std::endl;
    }

    m_renderer = SDL_CreateRenderer(m_window, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);

    m_viewport = {0, 0, p_w, p_h};
    SDL_RenderSetViewport(m_renderer, &m_viewport);

    m_basePath = SDL_GetBasePath();

    recalculateScalingFactors();
}

SDL_Texture* RenderWindow::loadTexture(const char* p_filePath) {
    SDL_Texture* texture = NULL;
    texture = IMG_LoadTexture(m_renderer, p_filePath);

    if (texture == NULL) {
        std::cout << "Failed to load texture. Error:\n" << SDL_GetError() << std::endl;
    }

    return texture;
}

void RenderWindow::cleanUp() {
    SDL_DestroyWindow(m_window);
}

void RenderWindow::clear() {
    SDL_RenderClear(m_renderer);
}

void RenderWindow::render(Entity& p_entity) {
    SDL_Rect* sRect = nullptr;
    SDL_Rect* dRect = nullptr;
    
    if (p_entity.useSource()) {
        sRect = p_entity.getSrc();
    }

    if (p_entity.useDestination()) {
        dRect = p_entity.getDst();
    }

    SDL_RenderCopy(m_renderer, p_entity.getTex(), sRect, dRect);
}

void RenderWindow::render(SDL_Texture* p_texture, SDL_Rect& p_srcrect, SDL_Rect& p_dstrect, bool p_shouldFlip) {
    SDL_Rect srcrect = p_srcrect;
    SDL_FRect renderRect;
    renderRect.x = static_cast<int>(p_dstrect.x * m_scaleX);
    renderRect.y = static_cast<int>(p_dstrect.y * m_scaleY);
    renderRect.w = static_cast<int>(p_dstrect.w * m_scaleX);
    renderRect.h = static_cast<int>(p_dstrect.h * m_scaleY);

    SDL_RenderCopyExF(m_renderer, p_texture, &srcrect, &renderRect, 0.0, nullptr, p_shouldFlip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE);
}

void RenderWindow::renderBackground(SDL_Texture* p_texture, SDL_Rect* p_srcrect, SDL_Rect* p_dstrect) {
    SDL_RenderCopy(m_renderer, p_texture, p_srcrect, p_dstrect);
}

void RenderWindow::display() {
    SDL_RenderPresent(m_renderer);
}

void RenderWindow::fullscreen() {
    Uint32 fullscreenFlag = SDL_WINDOW_FULLSCREEN_DESKTOP;
    bool isFullscreen = SDL_GetWindowFlags(m_window) & fullscreenFlag;
    SDL_SetWindowFullscreen(m_window, isFullscreen ? 0 : fullscreenFlag);
}

const std::string& RenderWindow::getBasePath() {
    return m_basePath;
}

SDL_Renderer* RenderWindow::getRenderer() {
     return m_renderer;
}

const int RenderWindow::getWindowHeight() const {
    return m_windowHeight;
}

const int RenderWindow::getWindowWidth() const {
    return m_windowWidth;
}

const SDL_Rect RenderWindow::GetWindowRect() const {
    SDL_Rect rect{};
    SDL_GetWindowSize(m_window, &rect.w, &rect.h);

    return rect;
}

void RenderWindow::recalculateScalingFactors() {
    SDL_Rect windowRect = GetWindowRect();
    m_scaleX = (float)windowRect.w / PRIMARY_RES_W;
    m_scaleY = (float)windowRect.h / PRIMARY_RES_H;
}

const Vector2f RenderWindow::getScalingFactors() const {
    Vector2f scalingFactors(m_scaleX, m_scaleY);
    return scalingFactors;
}