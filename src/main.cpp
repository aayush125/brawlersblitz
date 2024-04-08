#include <iostream>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>

#include "RenderWindow.hpp"
#include "Utils.hpp"
#include "Entity.hpp"
#include "LoadEntity.hpp"
#include "spritesheet.hpp"
#include "Characters.hpp"
#include "LoadCharacter.hpp"
#include "Background.hpp"
#include "ResourceManager.hpp"
#include "Inputs.hpp"
#include "Physics.hpp"


// TODO: An init() function that initializes variables like scaleX and scaleY and other stuff

int main(int argc, char* argv[]) {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        std::cout << "Error initializing SDL:\n" << SDL_GetError() << std::endl;

        return 0;
    }

    if (!(IMG_Init(IMG_INIT_PNG))) {
        std::cout << "IMG_Init has failed. Error:\n" << IMG_GetError() << std::endl;

        return 0;
    }

    if (TTF_Init() != 0) {
        std::cout << "TTF_Init has failed. Error:\n" << TTF_GetError() << std::endl;

        return 0;
    }

    RenderWindow window("Brawlers' Blitz v1.0", 1280, 720);

    ResourceManager manager(window);    

    Background background(level::levels::LEVELONE, manager);
    background.addCollider(SDL_Rect{0, 992, 1920, 94});

    MartialHero martialChar(&manager, 380, CharacterBase::characterIDs::PLAYERONE);
    MartialHero martialCharTwo(&manager, 380, CharacterBase::characterIDs::PLAYERTWO);
    
    SDL_Rect ground = background.getCollider(Background::collionBoxes::GROUND);

    window.recalculateScalingFactors();

    Physics physics(ground.y);

    SDL_Point charOneInitialPosition = {55, (ground.y - (martialChar.getCharDim().h * SPRITE_SCALE_FACTOR))};
    SDL_Point charTwoInitialPosition = {550, (ground.y - (martialCharTwo.getCharDim().h * SPRITE_SCALE_FACTOR))};

    martialChar.setInitialPosition(charOneInitialPosition);
    martialCharTwo.setInitialPosition(charTwoInitialPosition);
    
    SDL_Rect charOnePosition = {charOneInitialPosition.x, 
                                charOneInitialPosition.y, 
                                martialChar.getCharDim().w * SPRITE_SCALE_FACTOR, 
                                martialChar.getCharDim().h * SPRITE_SCALE_FACTOR};
    
    SDL_Rect charTwoPosition = {charTwoInitialPosition.x, 
                                charTwoInitialPosition.y, 
                                martialCharTwo.getCharDim().w * SPRITE_SCALE_FACTOR, 
                                martialCharTwo.getCharDim().h * SPRITE_SCALE_FACTOR};
    
    martialChar.setPosition(charOnePosition);
    martialCharTwo.setPosition(charTwoPosition);

    try {
        martialChar.setSpritesheet(MartialHero::IDLE);
    } catch (const std::out_of_range& e) {
        std::cerr << "\nError setting spritesheet index for char one: \"" << e.what() << "\"\n";
    }

    try {
        martialCharTwo.setSpritesheet(MartialHero::IDLE);
    } catch (const std::out_of_range& e) {
        std::cerr << "\nError setting spritesheet index for char two: \"" << e.what() << "\"\n";
    }

    bool gameRunning = true;

    SDL_Event event;

    const float deltaTime = 1.0f / 60.0f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    while (gameRunning) {
        float newTime = utils::hireTimeInSeconds();

        charOnePosition = martialChar.getCurrentPosition();

        float frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= deltaTime) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                } else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window.recalculateScalingFactors();
                } else if (event.type == SDL_KEYDOWN) {
                    switch (event.key.keysym.sym) {
                        case SDLK_f:
                            window.fullscreen();
                            break;
                        default:
                            break;
                    }
                }
            }

            float dt = std::min(accumulator, deltaTime);

            try {
                martialChar.update(dt, window.GetWindowRect());
            } catch (std::out_of_range& e) {
                std::cerr << "Error updating character one: " << e.what() << '\n';
            }

            try {
                martialCharTwo.update(dt, window.GetWindowRect());
            } catch (std::out_of_range& e) {
                std::cerr << "Error updating character two: " << e.what() << '\n';
            }

            physics.gravity(martialChar, dt);
            physics.gravity(martialCharTwo, dt);

            const SDL_Rect rectOne = martialChar.getCharRect();
            const SDL_Rect rectTwo = martialCharTwo.getCharRect();
            // int charwidth = martialChar.getCurrentAnimIndex() == MartialHero::ATTACK1 ? 105 : 34;
            // int charwidthtwo = martialCharTwo.getCurrentAnimIndex() == MartialHero::ATTACK1 ? 105 : 34;
            int charwidth;
            if (martialChar.getCurrentAnimIndex() == MartialHero::ATTACK1 || martialCharTwo.getCurrentAnimIndex() == MartialHero::ATTACK1) {
                charwidth = CHAR_ATTACK_WIDTH;
            } else charwidth = CHAR_NORMAL_WIDTH;
            bool colliding = physics.collides(rectOne, rectTwo, charwidth, charwidth);

            accumulator -= dt;
        }

        // const float alpha = accumulator / deltaTime;

        window.clear();
        background.render();
        if (martialCharTwo.getCurrentAnimIndex() == MartialHero::ATTACK1) {
            martialCharTwo.playAnim();
            martialChar.playAnim();
        } else {
            martialChar.playAnim();
            martialCharTwo.playAnim();
        }
        window.display();
    }

    window.cleanUp();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
