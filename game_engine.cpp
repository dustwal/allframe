#include "game_engine.hh"

using namespace alengi;

int init() {
    al_init();
    return 0;
}

int close() {
    return 0;
}


// class Game definitions

Game::Game(GameState* gs_array, int size, int default_state) : 
    game_state_array(gs_array), 
    game_state_array_size(size), 
    default_state(gs_array[default_state]) {
    ALLEGRO_DISPLAY_MODE disp_data;
    al_get_display_mode(al_get_num_display_modes() - 1, &disp_data);
    al_set_new_display_flags(ALLEGRO_FULLSCREEN);
    display = al_create_display(disp_data.width, disp_data.height);
}

Game::Game(GameState* gs_array, int size, int default_state, int width, int height) : 
    game_state_array(gs_array),
    game_state_array_size(size),
    default_state(gs_array[default_state]), 
    display(al_create_display(width, height)) 
{}

Game::~Game() {
    al_destroy_display(display);
}
