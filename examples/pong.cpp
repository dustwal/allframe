// pong.cpp
// Dustin Walde 2015
// Two player pong with controllers or keyboard

#include "pong.h"

using namespace pong;

// ###########
//    GAME
// ###########

// names:
// ball -> go_ball : Ball
// playerN -> go_pN : Player
// controller -> go_mom : ControlController, PongLogic

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
    if (press) {
        Player* player = players[id];
        if (movement) player->set_velocity(1.0f);
        else player->set_velocity(-1.0f);
    } else if (!pmap[id][0] && !pmap[id][1]) {
        players[id]->set_velocity(0f);
    }
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
    Point pp = parent->get_position();
    float height = ((Player*) behave("ob_pplayer"))->get_height();
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+player_width, pp.y+height, color);
}

void BallPen::draw() {
    Point pp = parent->get_position();
    float radius = ((Ball*) behave("ob_pball"))->get_radius();
    al_draw_filled_circle(pp.x, pp.y, radius, color);
}
