#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>

#include "Entity.hpp"

Entity::Entity() {}

Entity::Entity(float p_x, float p_y, SDL_Texture* p_tex, SDL_Rect p_src, SDL_Rect p_dst, bool p_uSrc, bool p_uDst) : 
    m_x{p_x}, m_y{p_y}, m_tex{p_tex}, m_src{p_src}, m_dst{p_dst}, m_uSrc{p_uSrc}, m_uDst{p_uDst} {
}

SDL_Texture* Entity::getTex() {
    return m_tex;
}

SDL_Rect* Entity::getSrc() {
    return &m_src;
}

SDL_Rect* Entity::getDst() {
    return &m_dst;
}

bool Entity::useSource() {
    return m_uSrc;
}

bool Entity::useDestination() {
    return m_uDst;
}