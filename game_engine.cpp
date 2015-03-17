#include <iostream>

#include "game_engine.hh"

using namespace alengi;

int alengi::init() {
    return !al_init();
}

int alengi::close() {
    return 0;
}


GameState::GameState(ALLEGRO_DISPLAY* display) :
    display(display), 
    objects(new std::vector<GameObject>), 
    event_queue(al_create_event_queue()), 
    timer(al_create_timer(1.0)),
    is_close(false), event_map(new EventMap) {
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

    bool boo = true;
    while (!is_close) {
        boo = !boo;
        al_wait_for_event(event_queue, &event);

        ALLEGRO_EVENT_TYPE type = event.type;
        if (emap.find(type) != emap.end())
            (this->*emap[event.type])();

            // network events
            // object updates
            // controller updates
            // local game logic
            // netowrk sync
            // draw objects
        
        is_close = boo;
    }

    al_rest(1);
    return NULL;
}

void GameState::action_tick() {
    if (true)
        al_clear_to_color(al_map_rgb(155,155,0));
    else
        al_clear_to_color(al_map_rgb(0,200,155));
    al_flip_display();
}
