#include <iostream>

#include "game_frame.h"

using namespace allframe;

int allframe::init() {
    return !al_init();
}

int allframe::close() {
    //al_uninstall_system();
    return 0;
}


GameState::GameState(ALLEGRO_DISPLAY* display) :
    display(display), 
    objects(new std::unordered_map<std::string, GameObject>), 
    event_queue(al_create_event_queue()), 
    timer(al_create_timer(1.0/GameState::FRAME_RATE)),
    is_close(false), 
    next_state(NULL), 
    event_map(new EventMap),
    scene_color(al_map_rgb(0,0,0)) {
    
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
    
    destroy();    

    delete objects;
    delete event_map;
    
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

}

std::vector<GameObject*>* GameState::get_objects_of_behavior(std::string name) {
    auto bobjects = new std::vector<GameObject*>;
    for (auto it = objects->begin(); it != objects->end(); it++) {
        GameObject* obj = &(it->second);
        if (obj->has_behavior(name))
            bobjects->push_back(obj);
    }
    return bobjects;
}

std::vector<ObjectBehavior*>* GameState::get_behaviors_of_type(std::string name) {
    auto bobjects = new std::vector<ObjectBehavior*>;
    for (auto it = objects->begin(); it != objects->end(); it++) {
        GameObject* obj = &(it->second);
        if (obj->has_behavior(name))
            bobjects->push_back(obj->get_behavior(name));
    }
    return bobjects;
}

std::vector<GameObject*>* GameState::get_objects_of_type(std::string name) {
    auto bobjects = new std::vector<GameObject*>;
    for (auto it = objects->begin(); it != objects->end(); it++) 
        if (it->second.get_type_name() == name)
            bobjects->push_back(&(it->second));
    return bobjects;
}

std::string GameState::add_object(GameObject& object) {
    std::string object_name = object.get_unique_name();
    while (objects->find(object_name) != objects->end())
        object_name += " repeat";
    object.rename(object_name);
    //(*objects)[object_name] = object;
    return object_name;
}

void GameState::remove_object(std::string name) {
    objects->erase(name);
}

GameObject& GameState::get_object(std::string name) {
    return objects->find(name)->second;
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
    return next_state;
}

void GameState::action_tick() {

    // object updates
    for (auto it = objects->begin(); it != objects->end(); it++) 
        (it->second).object_update();

    // draw objects
    for (auto it = objects->begin(); it != objects->end(); it++) 
        (it->second).draw();
    al_flip_display();
    al_clear_to_color(scene_color);

}
