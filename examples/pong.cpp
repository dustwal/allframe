// Two player pong with controllers or keyboard

#include "pong.h"

using namespace pong;

// ###########
//    GAME
// ###########

// ###########
//  BEHAVIORS
// ###########

void ControlController::setup() {

}

void ControlController::update() {

}

// ###########
//  HANDLERS
// ###########

void JoystickController::event() {

}

void KeyboardController::event() {

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
