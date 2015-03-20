#include <cmath>
#include <cstdlib>

#include "common.h"

using namespace allframe;

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

void Clickable::mouse_down(ALLEGRO_EVENT mouse) {
    Point anchor = parent->get_position();
    double x = mouse.mouse.x - anchor.x;
    double y = mouse.mouse.y - anchor.y;
    Point result = point_rotate({x, y}, parent->get_rotation());
    if (bounds->is_within(result))
        is_pressed = true;
}

void Clickable::mouse_up(ALLEGRO_EVENT mouse) {
    if (!is_pressed) return;
    is_pressed = false;
    Point anchor = parent->get_position();
    Point result =  point_rotate({mouse.mouse.x-anchor.x, mouse.mouse.y-anchor.y}, 
                    parent->get_rotation());
    if (bounds->is_within(result))
       on_click();
}
