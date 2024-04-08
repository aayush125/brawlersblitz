#pragma once
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "Entity.hpp"
#include "RenderWindow.hpp"

void loadBackgroundEntity(int p_level, RenderWindow& p_window, Entity& p_entity);
void loadSpriteEntity(std::string p_sprite, RenderWindow& p_window, Entity& p_entity);