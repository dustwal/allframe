// pong.cpp
// Dustin Walde 2015
// Two player pong with controllers or keyboard

#include "pong.h"

using namespace pong;

// ###########
//    GAME
// ###########

// names:
// ball -> go_ball : Ball : parent -> table
// playerN -> go_pN : Player : parent -> table
// table -> go_table : Table
// controller -> go_mom : ControlController, PongLogic
//
// add controller last
// add table before children

// ###########
//  BEHAVIORS
// ###########

void ControlController::setup() {
    player_map = new std::map<uint64_t, Player*>();
    pressed = new std::map<uint64_t, bool*>();
}

void ControlController::destroy() {
    delete player_map;
    delete pressed;
}

void ControlController::map_player(uint64_t id, Player* player) {
    auto& players = *player_map;
    auto& pmap = *pressed;
    players[id] = player;
    pmap[id] = new bool[2];
    pmap[id][0] = false;
    pmap[id][1] = false;
}

void ControlController::action_axis(uint64_t id, float val) {
    auto& players = *player_map;
    if (players.find(id) == players.end()) return;
    Player* player = players[id];
    player->set_velocity(val);
}

void ControlController::action_button(uint64_t id, bool movement, bool press) {
    auto& players = *player_map;
    auto& pmap = *pressed;
    if (players.find(id) == players.end()) return;
    pmap[id][movement] = press;
    if (press) { // TODO? move on press
        Player* player = players[id];
        if (movement) player->set_velocity(1.0f);
        else player->set_velocity(-1.0f);
    } else if (!pmap[id][0] && !pmap[id][1]) {
        players[id]->set_velocity(0f);
    }
}

void Player::setup() {
    velocity = 0f;
    parent_bounds = ((Table*)(parent->parent->get_behavior("ob_ptable")))->bounds;
    pix_wid = parent_bounds.x*player_wid;
    pix_hig = parent_bounds.y*player_hig;
}

void Player::update() {
    double py = parent->position.y;
    double newy = py + ppixel_speed*velocity;
    if (newy < 0) newy = 0;
    double maxy = parent_bounds.y-pix_hig;
    if (newy > maxy) newy = maxy;
    parent->position.y = newy;
}

void Player::set_velocity(float vel) {
    if (vel > 1.0f) velocity = 1.0f;
    else if (vel < -1.0f) velocity = -1.0f;
    else velocity = vel;
}

float Player::get_height() {
    return pix_hig;
}

float Player::get_width() {
    return pix_wid;
}

float Ball::setup() {
    velocity = Point(0,0);
    radius = .05f;
    parent_bounds = ((Table*)(parent->parent->get_behavior("ob_ptable")))->bounds;
    pix_rad = radius*std::min(parent_bounds.x, parent_bounds.y);
}

float Ball::update() {
    Point ppos = parent->position;
    Point newpos = ppos;
    newpos.x += velocity.x;
    newpos.y += velocity.y;
    double maxy = parent_bounds.y-pix_rad
    if (newpos.y <= pix_rad || newpos.y >= maxy) {
        velocity.y = -velocity.y;
        if (newpos.y < pix_rad) newpos.y = pix_rad;
        else if (newpos.y > maxy) newpos.y = maxy;
    }
}

float Ball::get_radius() {
    return pix_rad;
}

// ###########
//  HANDLERS
// ###########

void JoystickController::event(ALLEGRO_EVENT& e) {

    if (e.type == ALLEGRO_EVENT_JOYSTICK_AXIS && e.joystick.axis == 1) {
        auto& cc = *(parent->get_object("go_mom")->get_behavior("ob_contcont"));
        cc.action_axis((uint64_t) e.joystick.id, e.joystick.pos);
    } else if (e.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
        //TODO
    }

}

void KeyboardController::event(ALLEGRO_EVENT& e) {

    auto& cc = *(parent->get_object("go_mom")->get_behavior("ob_contcont"));
    if (e.type == ALLEGRO_EVENT_KEY_DOWN) {
        switch (e.keyboard.keycode) {
            case ALLEGRO_KEY_ESCAPE:
                parent->signal_close();
                break;
            case ALLEGRO_KEY_W:
                cc.action_button(1, true, true);
                break;
            case ALLEGRO_KEY_S:
                cc.action_button(1, false, true);
                break;
            case ALLEGRO_KEY_SPACE:
                //TODO
                break;
            case ALLEGRO_KEY_UP:
                cc.action_button(2, true, true);
                break;
            case ALLEGRO_KEY_DOWN:
                cc.action_button(2, false, true);
                break;
            case ALLEGRO_KEY_RSHIFT:
                //TODO
                break;
        }
    } else if (e.type == ALLEGRO_EVENT_KEY_UP) {
        switch (e.keyboard.keycode) {
            case ALLEGRO_KEY_W:
                cc.action_button(1, true, false);
                break;
            case ALLEGRO_KEY_S:
                cc.action_button(1, false, false);
                break;
            case ALLEGRO_KEY_UP:
                cc.action_button(2, true, false);
                break;
            case ALLEGRO_KEY_DOWN:
                cc.action_button(2, false, false);
                break;
        }
    }

}

// ###########
//    PENS
// ###########

void PlayerPen::draw() {
    Point& pp = parent->get_global_position();
    Player* player = (Player*) behave("ob_pplayer");
    float height = player->get_height();
    float widht = player->get_width();
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+width, pp.y+height, color);
}

void BallPen::draw() {
    Point& pp = parent->get_global_position();
    float radius = ((Ball*) behave("ob_pball"))->get_radius();
    al_draw_filled_circle(pp.x, pp.y, radius, color);
}

void TablePen::draw() {
    Point& pp = parent->get_global_position();
    Point& bounds = ((Table*) behave("ob_ptable"))->bounds;
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+bounds.x, pp.y+bounds.y, color);
}
