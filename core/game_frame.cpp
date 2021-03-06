#include <allegro5/allegro_font.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_primitives.h>
#include <allegro5/allegro_ttf.h>
#include <cmath>
#include <iostream>
#include <memory>
#include <unordered_set>

#include "game_frame.h"

using namespace allframe;

bool allframe::init() {
    int status = al_init();
    if (!status) {
        std::cerr << "ERROR  : Allegro initialization failed" << std::endl;
        return false;
    }
    std::cout << "STATUS : Allegro initialized" << std::endl;
    status = al_init_primitives_addon();
    if (!status) {
        std::cerr << "ERROR  : Allegro primitives addon initialization failed" << std::endl;
        return false;
    }
    std::cout << "STATUS : Allegro primitives addon initialized" << std::endl;
    al_init_font_addon();
    std::cout << "STATUS : Allegro font addon initialized" << std::endl;
    status = al_init_ttf_addon();
    if (!status) {
        std::cerr << "ERROR  : Allegro ttf addon initialization failed" << std::endl;
        return false; 
    }
    std::cout << "STATUS : Allegro ttf addon initialized" << std::endl;
    status = al_init_image_addon();
    if (!status) {
        std::cerr << "ERROR  : Allegro image addon initialization failed" << std::endl;
        return false;
    }
    std::cout << "STATUS : Allegro image addon initialized" << std::endl;
    return true;
}

bool allframe::close() {
    std::cout << "STATUS : shutting down Allegro image addon" << std:: endl;
    al_shutdown_image_addon();
    std::cout << "STATUS : shutting down Allegro ttf addon" << std::endl;
    al_shutdown_ttf_addon();
    std::cout << "STATUS : shutting down Allegro font addon" << std::endl;
    al_shutdown_font_addon();
    std::cout << "STATUS : shutting down Allegro primitives addon" << std::endl;
    al_shutdown_primitives_addon();
    std::cout << "STATUS : shutting down Allegro" << std::endl;
    //al_uninstall_system(); // TODO WHY DOES THIS CRASH!!
    return 0;
}

void ObjectBehavior::set_parent(GameObject* parent) {
    this->parent = parent;
    this->parent_state = parent->get_parent_state();
}

ObjectBehavior*  ObjectBehavior::get_behavior(std::string name) const {
    return parent->get_behavior(name);
}

GameObject* ObjectBehavior::get_object(std::string name) const {
    return parent_state->get_object(name);
}

ObjectBehavior* ObjectBehavior::object_behavior(std::string obname, std::string bname) const {
    return parent_state->get_object(obname)->get_behavior(bname);
}

std::vector<GameObject*>* ObjectBehavior::get_objects_of_behavior(std::string name) const {
    return parent_state->get_objects_of_behavior(name);
}

std::vector<ObjectBehavior*>* ObjectBehavior::get_behaviors_of_type(std::string name) const {
    return parent_state->get_behaviors_of_type(name);
}

Point& ObjectBehavior::get_position() const {
    return parent->position;
}

ObjectBehavior* Pen::get_behavior(std::string name) const {
    return parent->get_behavior(name);
}

MultiPen::~MultiPen() {
    for (auto it = pens->begin(); it != pens->end(); it++)
        delete *it;
    delete pens;
}

void MultiPen::draw() {
    for (auto it = pens->begin(); it != pens->end(); it++)
        (*it)->draw();
}

void MultiPen::add_pen(Pen* pen) {
    if (pen == NULL) { 
        std::cerr << "WARNING: in MultiPen::add_pen(Pen*): added NULL pen" << std::endl;
        return;
    }
    if (parent != NULL)
        pen->set_parent(parent);
    else
        std::cerr << "WARNING: in MultiPen::add_pen(Pen*): adding pen before parent set" << std::endl;
    pens->push_back(pen);
}

void GameObject::set_pen(Pen* pen) {
    // if pens contulually set memory is not freed TODO
    parent_state->add_pen(pen);
    pencil = pen;
    pencil->set_parent(this);
}

void GameObject::set_global_rotation(double rotation) {
    if (parent == NULL)
        this->rotation = rotation;
    else
        this->rotation = std::fmod((4*M_PI+rotation-parent->get_global_rotation()),(2*M_PI));
}

void GameObject::set_global_position(const Point& point) {
    if (parent == NULL) {
        this->position = point;
    } else {
        Point global_pt = parent->get_global_position();
        this->position = {point.x-global_pt.x,point.y-global_pt.y};
    }
}

void TickEvent::event(ALLEGRO_EVENT& event) {
    // TODO skip draw phase when dequeueing starts to lag
    // ## look into timer.count function
    // object updates
    for (auto it = parent->get_begin(); it != parent->get_end(); it++)
        (it->second)->update();

    /*
    // draw objects
    for (auto it = parent->get_begin(); it != parent->get_end(); it++) 
        (it->second)->draw();

    al_flip_display();
    al_clear_to_color(parent->get_color());
    */
}

void CloseEvent::event(ALLEGRO_EVENT& event) {
    std::cout << "STATUS : exiting game" << std::endl;
    parent->signal_close();
}

GameState::GameState(ALLEGRO_DISPLAY* display) :
    next_state(NULL), 
    scene_color(al_map_rgb(0,0,0)),
    objects(new std::unordered_map<std::string, GameObject>), 
    names_to_z(new std::unordered_map<std::string, double>),
    sorted_objects(new std::map<double, GameObject*>),
    display(display), 
    event_queue(al_create_event_queue()), 
    timer(al_create_timer(1.0/GameState::FRAME_RATE)),
    is_close(false), 
    event_map(new std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler*>),
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
    EventHandler* handler = new CloseEvent();
    handler->set_parent_state(this);
    evmap[ALLEGRO_EVENT_DISPLAY_CLOSE] = handler;
    timer_event = new TickEvent();
    timer_event->set_parent_state(this);
}


GameState::~GameState() {
    al_destroy_event_queue(event_queue);
    al_destroy_timer(timer);

    for (auto it = pens->begin(); it != pens->end(); it++)
        delete *it;

    std::unordered_set<EventHandler*> deleted_handlers;
    for (auto it = event_map->begin(); it != event_map->end(); it++) {
        if (deleted_handlers.find(it->second) != deleted_handlers.end()) {
            deleted_handlers.insert(it->second);
            delete it->second;
        }
    }

    delete timer_event;
    delete objects;
    delete event_map;
    delete pens;
    delete names_to_z;
    delete sorted_objects;

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

// TODO: behavior on nonexistent object??? fail or place on bottom..
std::string GameState::add_object_topof(std::string name, std::string topof) {
    auto it = names_to_z->find(topof);
    if (it == names_to_z->end()) {
        std::cerr << "ERROR  : no object " << topof << " to place on top of." << std::endl;
        return "";
    }
    while (objects->find(name) != objects->end())
        name += " repeat";
    auto it_now = sorted_objects->find(it->second);
    auto it_next = std::next(it_now,1);
    double z = (it_now->first) + 1;
    if (it_next != sorted_objects->end())
        z = (z-1 + it_next->first) / 2.0;
    GameObject obj(this, name);
    objects->insert(std::pair<std::string, GameObject>(name, obj));
    (*names_to_z)[name] = z;
    (*sorted_objects)[z] = &(objects->find(name)->second);
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

// vector iterator contructor can be used with arrays
void GameState::add_event_handler(EventHandler* handle, std::vector<ALLEGRO_EVENT_TYPE>& events) {
    auto& emap = *event_map;
    handle->set_parent_state(this);
    for (ALLEGRO_EVENT_TYPE event : events) {
        emap[event] = handle;
    }
}

GameState* GameState::game_loop() {
    // manage events
    ALLEGRO_EVENT event;
    std::unordered_map<ALLEGRO_EVENT_TYPE, EventHandler*>& emap = *event_map;
    al_start_timer(timer);

    while (!is_close) {
        al_wait_for_event(event_queue, &event);
        ALLEGRO_EVENT_TYPE type = event.type;
        if (event.type == ALLEGRO_EVENT_TIMER && event.timer.source == timer) {
            timer_event->event(event);
        } else if (emap.find(type) != emap.end())
            emap[event.type]->event(event);

        if (al_is_event_queue_empty(event_queue)) {
            // draw objects
            for (auto it = get_begin(); it != get_end(); it++) 
                (it->second)->draw();

            al_flip_display();
            al_clear_to_color(get_color());
        }

    }
    destroy();
    return next_state;
}
