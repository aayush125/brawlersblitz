#include "Inputs.hpp"

const Uint8* get_keyboard_state() {
    return SDL_GetKeyboardState(NULL);
}

void handle_inputs(SDL_Event& event) {
}