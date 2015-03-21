#include <allegro5/allegro_primitives.h>
#include <iostream>

#include "game_frame.h"

using namespace allframe;

int allframe::init() {
    return !al_init() || !al_init_primitives_addon();
}

int allframe::close() {
    //al_uninstall_system();
    return 0;
}

void GameObject::set_pen(Pen* pen) {
    // if pens contulually set memory is not freed TODO
    parent_state->add_pen(pen);
    pencil = pen;
    pencil->set_parent(this);
}

void TickEvent::event(ALLEGRO_EVENT& event) {

    //std::cout<< "tick" << std::endl;
    // object updates
    /*
    for (auto it = parent->objects->begin(); it != parent->objects->end(); it++) 
        (it->second).update();
    */
    for (auto it = parent->get_begin(); it != parent->get_end(); it++) {
        //std::cout << it->first << std::endl;
        (it->second)->update();
    }

    //std::cout<< "tick middle" << std::endl;
    // draw objects
    //for (auto it = parent->objects->begin(); it != parent->objects->end(); it++) 
    //    (it->second).draw();
    for (auto it = parent->get_begin(); it != parent->get_end(); it++) { 
        //std::cout << it->first << std::endl;
        (it->second)->draw();
    }
    al_flip_display();
    al_clear_to_color(parent->get_color());
    
    //std::cout<< "tick end" << std::endl;

}

void CloseEvent::event(ALLEGRO_EVENT& event) {
    std::cout << "exiting" << std::endl;
    parent->signal_close();
}

GameState::GameState(ALLEGRO_DISPLAY* display) :
    objects(new std::unordered_map<std::string, GameObject>), 
    names_to_z(new std::unordered_map<std::string, double>),
    sorted_objects(new std::map<double, GameObject*>),
    display(display), 
    event_queue(al_create_event_queue()), 
    timer(al_create_timer(1.0/GameState::FRAME_RATE)),
    is_close(false), 
    next_state(NULL), 
    event_map(new std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler*>),
    scene_color(al_map_rgb(0,0,0)),
    pens(new std::vector<Pen*>) {
    
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
    

    auto& evmap = *event_map;
    evmap[ALLEGRO_EVENT_DISPLAY_CLOSE] = new CloseEvent(this);
    evmap[ALLEGRO_EVENT_TIMER] = new TickEvent(this);
}


GameState::~GameState() {
    destroy();    

    for (auto it = pens->begin(); it != pens->end(); it++)
        delete &(*it);

    for (auto it = event_map->begin(); it != event_map->end(); it++)
        delete it->second;

    delete objects;
    delete event_map;
    delete pens;
    delete names_to_z;
    delete sorted_objects;

    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);
    std::cout << "end destructor" << std::endl;

}

std::vector<GameObject*>* GameState::get_objects_of_behavior(std::string name) const {
    auto bobjects = new std::vector<GameObject*>;
    for (auto it = objects->begin(); it != objects->end(); it++) {
        GameObject* obj = &(it->second);
        if (obj->has_behavior(name))
            bobjects->push_back(obj);
    }
    return bobjects;
}

std::vector<ObjectBehavior*>* GameState::get_behaviors_of_type(std::string name) const {
    auto bobjects = new std::vector<ObjectBehavior*>;
    for (auto it = objects->begin(); it != objects->end(); it++) {
        GameObject* obj = &(it->second);
        if (obj->has_behavior(name))
            bobjects->push_back(obj->get_behavior(name));
    }
    return bobjects;
}

std::string GameState::add_object(std::string name) {
    while (objects->find(name) != objects->end())
        name += " repeat";
    GameObject obj(this, name);
    objects->insert(std::pair<std::string, GameObject>(name, obj));
    GameObject* obj_ptr = &(objects->find(name)->second);
    double z = 0;
    if (sorted_objects->size() != 0)
        z = (sorted_objects->rbegin()->first) + 1;
    (*names_to_z)[name] = z;
    (*sorted_objects)[z] = obj_ptr;
    return name;
}

std::string GameState::add_object_topof(std::string name, std::string topof) {
    auto it = names_to_z->find(topof);
    if (it == names_to_z->end()) {
        std::cerr << "no object " << topof << " to place on top of." << std::endl;
        return "";
    }
    while (objects->find(name) != objects->end())
        name += " repeat";
    auto it_now = sorted_objects->find(it->second);
    auto it_next = std::next(it_now,1);
    double z = (it_now->first) + 1;
    if (it_next != sorted_objects->end())
        z = (z-1 + it_next->first) / 2.0;
    std::cout << name << " " << z << std::endl;
    GameObject obj(this, name);
    objects->insert(std::pair<std::string, GameObject>(name, obj));
    (*names_to_z)[name] = z;
    (*sorted_objects)[z] = &(objects->find(name)->second);
    std::cout << sorted_objects->size() << std::endl;
    return name;
}

void GameState::remove_object(std::string name) {
    auto it = objects->find(name);
    if (it == objects->end())
        return;
    objects->erase(name);
    double z = names_to_z->find(name)->second;
    names_to_z->erase(name);
    sorted_objects->erase(sorted_objects->find(z));
}

GameObject* GameState::get_object(std::string name) const {
    auto it = objects->find(name);
    if (it == objects->end()) return NULL;
    return &(it->second);
}


GameState* GameState::game_loop() {
    // manage events
    //std::cout << "game_loop" << std::endl;
    ALLEGRO_EVENT event;
    std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler*>& emap = *event_map;
    al_start_timer(timer);

    while (!is_close) {
        //std::cout << "loop" << std::endl;
        al_wait_for_event(event_queue, &event);
        ALLEGRO_EVENT_TYPE type = event.type;
        if (emap.find(type) != emap.end())
            emap[event.type]->event(event);
    }

    al_rest(1);
    return next_state;
}
