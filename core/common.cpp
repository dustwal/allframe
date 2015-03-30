#include <allegro5/allegro_primitives.h>
#include <cmath>
#include <cstdlib>

#include "common.h"

using namespace allframe;

ALLEGRO_COLOR Colors::BLACK = al_map_rgb(0,0,0);
ALLEGRO_COLOR Colors::WHITE = al_map_rgb(255,255,255);
ALLEGRO_COLOR Colors::RED   = al_map_rgb(255,0,0);
ALLEGRO_COLOR Colors::GREEN = al_map_rgb(0,255,0);
ALLEGRO_COLOR Colors::BLUE  = al_map_rgb(0,0,255);

ALLEGRO_COLOR Colors::random_color() {
    return al_map_rgb(std::rand()%256, std::rand()%256, std::rand()%256);
}

ALLEGRO_COLOR Colors::random_color_alpha() {
    return al_map_rgba(std::rand()%256, std::rand()%256, std::rand()%256, std::rand()%256);
}

Point allframe::point_rotate(const Point& point, double rotation) {
    double theta = std::atan2(point.y, point.x);
    double mag = std::sqrt(point.y*point.y+point.x*point.x);
    theta += rotation;
    return {mag*std::cos(theta), mag*std::sin(theta)};
}

bool BoxBounds::is_within(const Point& point) const {
    return  point.x >= 0 && point.x <= size.x && point.y >= 0 && point.y <= size.y;
}

bool BoxBounds::is_within(double x, double y) const {
    return is_within({x,y});
}

bool BoxBounds::is_within(int x, int y) const {
    return is_within({(double)x,(double)y});
}

bool CenterBoxBounds::is_within(const Point& point) const {
    return  point.x >= -size.x/2 && point.x <= size.x/2 &&
            point.y >= -size.y/2 && point.y <= size.y/2;
}

bool CenterBoxBounds::is_within(double x, double y) const {
    return is_within({x,y});
}

bool CenterBoxBounds::is_within(int x, int y) const {
    return is_within({(double)x,(double)y});
}

bool CircleBounds::is_within(const Point& point) const {
    return std::sqrt(point.x*point.x+point.y*point.y) <= radius;
}

bool CircleBounds::is_within(double x, double y) const {
    return is_within({x,y});
}

bool CircleBounds::is_within(int x, int y) const {
    return is_within({(double)x,(double)y});
}

bool OvalBounds::is_within(const Point& point) const {
    double mag = std::sqrt(point.x*point.x+point.y*point.y);
    double theta = std::atan2(point.y,point.x);
    return mag*std::cos(theta) <= radii.x && mag*std::sin(theta) <= radii.y;
}

bool OvalBounds::is_within(double x, double y) const {
    return is_within({x,y});
}

bool OvalBounds::is_within(int x, int y) const {
    return is_within({(double)x,(double)y});
}

void LeftClickable::mouse_down(ALLEGRO_EVENT& mouse) {
    Point anchor = parent->get_global_position();
    Point result =  point_rotate({mouse.mouse.x-anchor.x, mouse.mouse.y-anchor.y}, 
                    parent->get_global_rotation());
    if (bounds->is_within(result))
        is_pressed = true;
}

void Clickable::mouse_down(ALLEGRO_EVENT& event, unsigned button) {
    Point pp  = parent->get_global_position();
    Point rel = point_rotate({event.mouse.x-pp.x, event.mouse.y-pp.y}, 
                parent->get_global_rotation());
    if (bounds->is_within(rel))
        is_pressed |= (1 << button);
}

void LeftClickable::mouse_up(ALLEGRO_EVENT& mouse) {
    if (!is_pressed) return;
    is_pressed = false;
    Point anchor = parent->get_global_position();
    Point result =  point_rotate({mouse.mouse.x-anchor.x, mouse.mouse.y-anchor.y}, 
                    parent->get_global_rotation());
    if (bounds->is_within(result))
       on_click();
}

void Clickable::mouse_up(ALLEGRO_EVENT& event, unsigned button) {
    if (!(1 & (is_pressed >> button))) return;
    is_pressed &= ~(1 << button);
    Point pp  = parent->get_global_position();
    Point rel = point_rotate({event.mouse.x-pp.x, event.mouse.y-pp.y},
                parent->get_global_rotation()); 
    if (bounds->is_within(rel))
        on_click(button);
}

void LeftClickEvent::event(ALLEGRO_EVENT& event) {
    if (event.mouse.button != 1)
        return;
    auto clickables = parent->get_behaviors_of_type("af_left_clickable");
    for (auto it = clickables->begin(); it != clickables->end(); it++) {
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            ((LeftClickable*)(*it))->mouse_down(event);
        else
            ((LeftClickable*)(*it))->mouse_up(event);
    }
    delete clickables;
}

void Clickable::set_bounds(Bounds* bounds) {
    if (bounds != NULL) delete this->bounds;
    this->bounds = bounds;
}

void ClickEvent::event(ALLEGRO_EVENT& event) {
    auto clickables = parent->get_behaviors_of_type("af_clickable");
    for (auto it = clickables->begin(); it != clickables->end(); it++) {
        if (event.type == ALLEGRO_EVENT_MOUSE_BUTTON_DOWN)
            ((Clickable*)(*it))->mouse_down(event, event.mouse.button);
        else
            ((Clickable*)(*it))->mouse_up(event, event.mouse.button);
    }
    delete clickables;
}

void ButtonPen::draw() const {
    Point pp = parent->get_global_position();
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+100, pp.y+50, al_map_rgb(255,255,255));
    // draw text in center

}

void MenuStates::add_object_to_state(GameObject* object, std::string name) {
    auto it = states->find(name);
    if (it == states->end()) {
        std::vector<GameObject*> new_list;
        (*states)[name] = new_list;
        it = states->find(name);
    }
    it->second.push_back(object);
}

void MenuStates::set_state(std::string menu_set) {
    for (auto it = parent_state->get_begin(); it != parent_state->get_end(); it++) {
        it->second->set_visibility(false);
    }
    auto objs = (*states)[menu_set];
    for (auto it = objs.begin(); it != objs.end(); it++) {
        (*it)->set_visibility(true);
    }
}
