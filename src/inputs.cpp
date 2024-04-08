#include "Inputs.hpp"

const Uint8* getKeyboardState() {
    return SDL_GetKeyboardState(NULL);
}

void handleInputs(SDL_Event& event) {
}