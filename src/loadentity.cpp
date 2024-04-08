#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include <string>
#include <iostream>

#include "LoadEntity.hpp"

void loadBackgroundEntity(int p_level, RenderWindow& p_window, Entity& p_entity) {
    std::string backgroundFile;
    switch (p_level) {
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

    SDL_Texture* backgroundTexture = p_window.loadTexture((p_window.getBasePath() + "../assets/backgrounds/" + backgroundFile).c_str());
    
    p_entity = Entity(0, 0, backgroundTexture, SDL_Rect{}, SDL_Rect{}, false, false);

    std::cout << "Background loaded.\n";
}

void loadSpriteEntity(std::string p_sprite, RenderWindow& p_window, Entity& p_entity) {
    
    SDL_Texture* spriteTexture = p_window.loadTexture((p_window.getBasePath() + "../assets/sprites/" + p_sprite).c_str());
    
    int width, height;
    SDL_QueryTexture(spriteTexture, NULL, NULL, &width, &height);
    SDL_Rect sprite_src = {0, 0, width, height};
    SDL_Rect sprite_dst = sprite_src;

    p_entity = Entity(0, 0, spriteTexture, sprite_src, sprite_dst, true, true);

    std::cout << "Screen image loaded.\n";
}