#include <iostream>

#include "game_engine.hh"

using namespace alengi;

int alengi::init() {
    return !al_init();
}

int alengi::close() {
    return 0;
}


GameState::GameState() : 
    objects(new std::vector<GameObject>), 
    event_queue(al_create_event_queue()), 
    is_close(false), event_map(new EventMap) {
    if (event_queue == NULL) {
        std::cerr << "ERROR: couldn't create event queue" << std::endl;
        is_close = true;
    }
    setup();
}

GameState::~GameState() {
    delete objects;
    delete event_map;
    al_destroy_event_queue(event_queue);
}

void GameState::game_loop() {
    // manage events
    // network events
    // object updates
    // controller updates
    // local game logic
    // netowrk sync
    // draw objects
}
