#include <iostream>

#include "game_frame.h"

using namespace allframe;

int allframe::init() {
    return !al_init();
}

int allframe::close() {
    return 0;
}


GameState::GameState(ALLEGRO_DISPLAY* display) :
    display(display), 
    objects(new std::vector<GameObject>), 
    controllers(new std::vector<GameController>),
    event_queue(al_create_event_queue()), 
    timer(al_create_timer(1.0)),
    is_close(false), next_state(NULL), 
    event_map(new EventMap) {
    if (event_queue == NULL) {
        std::cerr << "ERROR: couldn't create event queue" << std::endl;
        is_close = true;
    }
    if (timer == NULL) {
        std::cerr << "ERROR: couldn't create timer" << std::endl;
        is_close = true;
    }
    al_register_event_source(event_queue, al_get_timer_event_source(timer));
    al_register_event_source(event_queue, al_get_display_event_source(display));
    EventMap& em = *event_map;
    em[ALLEGRO_EVENT_DISPLAY_CLOSE] = &GameState::signal_close;
    em[ALLEGRO_EVENT_TIMER] = &GameState::action_tick;
    setup();
}


GameState::~GameState() {
    delete objects;
    delete event_map;
    al_destroy_event_queue(event_queue);
}

GameState* GameState::game_loop() {
    // manage events
    ALLEGRO_EVENT event;
    EventMap& emap = *event_map;
    al_start_timer(timer);

    while (!is_close) {
        al_wait_for_event(event_queue, &event);

        ALLEGRO_EVENT_TYPE type = event.type;
        if (emap.find(type) != emap.end())
            (this->*emap[event.type])();
    }

    al_rest(1);
    return shut_down(next_state);
}

void GameState::action_tick() {

    // object updates
    for (auto it = objects->begin(); it != objects->end(); it++) 
        (*it).update();
    
    // controller updates
    for (auto it = controllers->begin(); it != controllers->end(); it++)
        (*it).update();

    // draw objects
    for (auto it = objects->begin(); it != objects->end(); it++) 
        (*it).draw();
    al_flip_display();
    al_clear_to_color(Colors::BLACK);
}
