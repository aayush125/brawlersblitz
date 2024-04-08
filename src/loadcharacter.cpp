#include "LoadCharacter.hpp"
#include "RenderWindow.hpp"
#include "Characters.hpp"
#include "ResourceManager.hpp"

void loadMartialHero(ResourceManager* p_manager, MartialHero& p_character) {
    p_character.addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/idle_mod_resized.png").c_str(), 
    1, 4, 8, {0, 1, 2, 3}, true));

    p_character.addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/run_mod2.png").c_str(), 
    1, 8, 8, {0, 1, 2, 3, 4, 5, 6, 7}, true));

    p_character.addSpritesheet(Spritesheet(p_manager, 
    (p_manager->getBasePathFromWindow() + "../assets/sprites/martial_hero/jump_mod.png").c_str(), 
    1, 2, 8, {0, 1}, true));
}