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
#include "Background.hpp"
#include "ResourceManager.hpp"
#include "Inputs.hpp"
#include "GameEngine.hpp"

// Next up: Implement health bar decreasing functionality and clean up code



// TODO: An init() function that initializes variables like scaleX and scaleY and other stuff
// TODO: Generalize getting the attack being performed by players and other stuff
// TODO: Jumping after pressing the attack key stops the attack. Fix it.

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

    GameEngine engine(manager, window);
    try {
        engine.add_player(CharacterBase::characterIDs::PLAYERONE, CharacterBase::PlayerClasses::MartialHero);
        engine.add_player(CharacterBase::characterIDs::PLAYERTWO, CharacterBase::PlayerClasses::Wizard);
    } catch (std::exception& e) {
        std::cerr << "Error occurred while adding player(s): " << e.what() << '\n';
    }

    engine.init(level::levels::LEVELONE);

    window.recalculate_scaling_factors();

    bool gameRunning = true;

    SDL_Event event;

    const float deltaTime = 1.0f / 60.0f;
    float accumulator = 0.0f;
    float currentTime = utils::hireTimeInSeconds();

    while (gameRunning) {
        float newTime = utils::hireTimeInSeconds();

        float frameTime = newTime - currentTime;

        currentTime = newTime;

        accumulator += frameTime;

        while (accumulator >= deltaTime) {
            while (SDL_PollEvent(&event)) {
                if (event.type == SDL_QUIT) {
                    gameRunning = false;
                } else if (event.window.event == SDL_WINDOWEVENT_RESIZED) {
                    window.recalculate_scaling_factors();
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

            engine.update_game(dt);
            accumulator -= dt;
        }

        // const float alpha = accumulator / deltaTime;

        window.clear();
        engine.render_scene();
        window.display();
    }

    window.cleanup();
    TTF_Quit();
    IMG_Quit();
    SDL_Quit();
    
    return 0;
}
