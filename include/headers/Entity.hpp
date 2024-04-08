#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <vector>

class Entity {
    private:
        float m_x, m_y;
        SDL_Texture* m_tex;
        SDL_Rect m_src;
        SDL_Rect m_dst;
        bool m_uSrc;
        bool m_uDst;
    public:
        Entity();
        Entity(float p_x, float p_y, SDL_Texture* p_tex, SDL_Rect p_src, SDL_Rect p_dst, bool p_uSrc, bool p_uDst);
        
        SDL_Texture* getTex();
        SDL_Rect* getSrc();
        SDL_Rect* getDst();
        bool useSource();
        bool useDestination();
};
