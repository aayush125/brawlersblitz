#include "Spritesheet.hpp"
#include "RenderWindow.hpp"
#include "Entity.hpp"
#include <SDL2/SDL_image.h>
#include <string>
#include <iostream>

Spritesheet::Spritesheet() {}

Spritesheet::Spritesheet(ResourceManager* manager, const char *path, int row, int column, int animationFps, std::vector<int> animationFrames, bool loop)
{
    m_managerref = manager;

    m_spritesheet_image = m_managerref->loadTex(path);

    m_animationFrames = animationFrames;
    m_animationSize = animationFrames.size();
    m_animationDelay = 1.0f / animationFps;
    m_currentAnimFrame = 0;
    m_animationTimer = 0.0f;
    m_shouldFlip = false;
    m_loop = loop;

    SDL_QueryTexture(m_spritesheet_image, NULL, NULL, &m_currentFrame.w, &m_currentFrame.h);
    m_currentFrame.w /= column;
    m_currentFrame.h /= row;

    m_spriteDimensions.w = m_currentFrame.w;
    m_spriteDimensions.h = m_currentFrame.h;
}

void Spritesheet::select_sprite(int x, int y)
{
    m_currentFrame.x = x * m_currentFrame.w;
    m_currentFrame.y = y * m_currentFrame.h;
}

void Spritesheet::draw_selected_sprite(SDL_Rect& position)
{
    m_managerref->renderTex(m_spritesheet_image, m_currentFrame, position, m_shouldFlip);
}

void Spritesheet::update(float deltaTime)
{
    if (!m_loop && m_currentAnimFrame == m_animationSize - 1) {
        m_finishedPlaying = true;
        m_currentAnimFrame = 0;
        return;
    }

    m_finishedPlaying = false;

    m_animationTimer += deltaTime;

    if (m_animationTimer >= m_animationDelay)
    {
        m_currentAnimFrame = (m_currentAnimFrame + 1) % m_animationFrames.size();
        m_animationTimer = 0.0f;
    }
}

void Spritesheet::playAnim(SDL_Rect& p_position, bool p_flipFlag)
{
    m_shouldFlip = p_flipFlag;

    select_sprite(m_animationFrames[m_currentAnimFrame], 0);
    draw_selected_sprite(p_position);
}

const SDL_Rect& Spritesheet::getSpriteDim() const {
    return m_spriteDimensions;
}

const int Spritesheet::getCurrentFrameNum() const {
    return m_currentAnimFrame;
}

const bool Spritesheet::finishedPlaying() const {
    return m_finishedPlaying;
}