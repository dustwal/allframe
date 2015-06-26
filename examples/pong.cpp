// pong.cpp
// Dustin Walde 2015
// Two player pong with controllers or keyboard

#include <cmath>

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

void Pong::setup() {

    std::cout << "STATUS : installing joystick" << std::endl;
    al_install_joystick();
    al_register_event_source(event_queue, al_get_joystick_event_source());
    std::cout << "STATUS : installing keyboard" << std::endl;
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    ALLEGRO_EVENT_TYPE events[] = {ALLEGRO_EVENT_JOYSTICK_AXIS, ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN};
    std::vector<ALLEGRO_EVENT_TYPE> v(events, events + sizeof(events)/sizeof(ALLEGRO_EVENT_TYPE));
    add_event_handler(new JoystickController, v);
    events[0] = ALLEGRO_EVENT_KEY_DOWN;
    events[1] = ALLEGRO_EVENT_KEY_UP;
    v = std::vector<ALLEGRO_EVENT_TYPE>(events, events + sizeof(events)/sizeof(ALLEGRO_EVENT_TYPE));
    add_event_handler(new KeyboardController, v); 

    std::string name = add_object("go_table");
    GameObject* obj = get_object(name);
    if (obj == NULL) {
        std::cerr << "ERROR  : problem initializing object" << std::endl;
        signal_close();
        return;
    }
    obj->set_pen(new TablePen);
    obj->add_behavior(new Table);

    name = add_object("go_ball");
    GameObject* obj2 = get_object(name);
    if (obj2 == NULL) {
        std::cerr << "ERROR  : problem initializing object" << std::endl;
        signal_close();
        return;
    }
    obj2->set_parent_object(obj);
    obj2->set_pen(new BallPen);
    obj2->add_behavior(new Ball);

    name = add_object("go_mom");
    obj2 = get_object(name);
    if (obj == NULL) {
        std::cerr << "ERROR  : problem initializing object" << std::endl;
        signal_close();
        return;
    }
    obj2->add_behavior(new PongLogic);
    obj2->add_behavior(new ControlController);
    std::out << "STATUS : done initializing" << std::endl;
}

void Pong::destroy() {
    std::out << "STATUS : closing game" << std::endl;
    al_uninstall_keyboard();
    al_uninstall_joystick();
}
    
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
        players[id]->set_velocity(0.0f);
    }
}

void Player::setup() {
    velocity = 0.0f;
    parent_bounds = ((Table*)(parent->parent->get_behavior("ob_ptable")))->bounds;
    pix_wid = parent_bounds.x*player_wid;
    pix_hig = parent_bounds.y*player_hig;
    bounds = new BoxBounds({pix_wid, pix_hig});
}

void Player::update() {
    double py = parent->position.y;
    double newy = py + ppixel_speed*velocity;
    if (newy < 0) newy = 0;
    double maxy = parent_bounds.y-pix_hig;
    if (newy > maxy) newy = maxy;
    parent->position.y = newy;
}

void Player::destroy() {
    delete bounds;
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

void Ball::setup() {
    velocity.x = 0;
    velocity.y = 0;
    radius = .01f;
    parent_bounds = ((Table*)(parent->parent->get_behavior("ob_ptable")))->bounds;
    pix_rad = radius*std::min(parent_bounds.x, parent_bounds.y);
    parent->position.x = parent_bounds.x/2;
    parent->position.y = parent_bounds.y/2;
}

void Ball::update() {
    Point ppos = parent->position;
    Point newpos = ppos;
    if (velocity.x > max_v) velocity.x = max_v;
    if (velocity.y > max_v) velocity.y = max_v;
    newpos.x += velocity.x;
    newpos.y += velocity.y;
    double maxy = parent_bounds.y-pix_rad;
    if (newpos.y <= pix_rad || newpos.y >= maxy) {
        velocity.y = -velocity.y;
        if (newpos.y < pix_rad) newpos.y = pix_rad;
        else if (newpos.y > maxy) newpos.y = maxy;
    }
}

float Ball::get_radius() {
    return pix_rad;
}

void PongLogic::setup() {
    num_players = 2;
    registered = 0;
    scores = new int[2];
    scores[0] = 0;
    scores[1] = 0;
    last_score = 10;
    ids = new std::set<uint64_t>();
    ball_ready = false;
}

void PongLogic::update() {
    if (registered < num_players) return;

    std::cout << "payers entered" << std::endl;

    if (ball_ready) { //TODO add buttons
        start_ball();
        return;
    }

    auto& ball = *((Ball*) parent->parent_state->get_object("go_ball")->get_behavior("ob_pball"));
    auto& p1 = *((Player*) parent->parent_state->get_object("go_p1")->get_behavior("ob_pplayer"));
    auto& p2 = *((Player*) parent->parent_state->get_object("go_p2")->get_behavior("ob_pplayer"));
    auto& table = *((Table*) parent->parent_state->get_object("go_table")->get_behavior("ob_ptable"));

    ball.velocity.x *= 1.01;
    ball.velocity.y *= 1.01;

    if (p1.bounds->is_within(ball.parent->position)) {
        ball.velocity.x = -ball.velocity.x;
        ball.parent->position.x = p1.parent->position.x+p1.get_width()+ball.get_radius();
    } else if (p2.bounds->is_within(ball.parent->position)) {
        ball.velocity.x = -ball.velocity.x;
        ball.parent->position.x = p2.parent->position.x-ball.get_radius();
    } else if (ball.parent->position.x <= ball.get_radius()) {
        scores[1] += 1;
        if (scores[1] == 7) game_over();
        last_score = 1;
        ball_reset();
    } else if (ball.parent->position.x >= table.bounds.y-ball.get_radius()) {
        scores[0] += 1;
        if (scores[0] == 7) game_over();
        last_score = 0;
        ball_reset();
    }
}

void PongLogic::destroy() {
    delete scores;
    delete ids;
}

void PongLogic::action(uint64_t id) {
    if (registered < num_players) {
        if (ids->find(id) == ids->end()) {
            auto& cc = *((ControlController*) parent->get_behavior("ob_contcont"));
            cc.map_player(id, new_player()); //TODO check null
        }
    } // TODO release ball
}

void PongLogic::ball_reset() {
    Point bbounds =((Table*) parent->parent_state->get_object("go_table")->get_behavior("ob_ptable"))->bounds;
    auto& ball = *((Ball*)(parent->parent_state->get_object("go_ball")->get_behavior("ob_pball")));
    ball.parent->position.x = bbounds.x/2;
    ball.parent->position.y = bbounds.y/2;
    ball.velocity.x = 0;
    ball.velocity.y = 0;
}

void PongLogic::game_over() {
    parent->parent_state->signal_close();
}

Player* PongLogic::new_player() {
    registered++;
    std::string name = parent->parent_state->add_object(std::string("go_p") + std::to_string(registered));
    GameObject* obj = parent->parent_state->get_object(name);
    if (obj == NULL) return NULL;
    obj->set_parent_object(parent->parent_state->get_object("go_mom"));
    obj->set_pen(new PlayerPen);
    obj->add_behavior(new Player);
    if (registered % 2 == 1) 
        obj->position.x = .1*al_get_display_width(parent->parent_state->get_display());
    else 
        obj->position.x = .85*al_get_display_width(parent->parent_state->get_display());
    obj->position.y = .45*al_get_display_height(parent->parent_state->get_display());
    return (Player*) obj->get_behavior("ob_pplayer");
}

void PongLogic::start_ball() {
    auto& ball = *((Ball*)(parent->parent_state->get_object("go_ball")->get_behavior("ob_pball")));
    if (last_score % 2 == 1) ball.velocity.x = 5;
    else ball.velocity.x = -5;
    ball.velocity.y = std::rand()*10 - 5;
}

void Table::setup() {
    int hig = al_get_display_height(parent->parent_state->get_display());
    int wid = al_get_display_width(parent->parent_state->get_display());
    bounds.x = .9*wid;
    bounds.y = .9*hig;
    parent->position.x = .1*wid;
    parent->position.y = .1*hig;
}

// ###########
//  HANDLERS
// ###########

void JoystickController::event(ALLEGRO_EVENT& e) {

    if (e.type == ALLEGRO_EVENT_JOYSTICK_AXIS && e.joystick.axis == 1) {
        auto& cc = *((ControlController*) parent->get_object("go_mom")->get_behavior("ob_contcont"));
        cc.action_axis((uint64_t) e.joystick.id, e.joystick.pos);
    } else if (e.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN && e.joystick.button == 1) {
        auto& pl = *((PongLogic*) parent->get_object("go_mom")->get_behavior("ob_ponglogic"));
        pl.action((uint64_t) e.joystick.id);
    }

}

void KeyboardController::event(ALLEGRO_EVENT& e) {

    auto& cc = *((ControlController*) parent->get_object("go_mom")->get_behavior("ob_contcont"));
    if (e.type == ALLEGRO_EVENT_KEY_DOWN) {
        PongLogic* pl = NULL;
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
                pl = (PongLogic*) parent->get_object("go_mom")->get_behavior("ob_ponglogic");
                pl->action(1);
                break;
            case ALLEGRO_KEY_UP:
                cc.action_button(2, true, true);
                break;
            case ALLEGRO_KEY_DOWN:
                cc.action_button(2, false, true);
                break;
            case ALLEGRO_KEY_RSHIFT:
                pl = (PongLogic*) parent->get_object("go_mom")->get_behavior("ob_ponglogic");
                pl->action(2);
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
    Point pp = parent->get_global_position();
    Player* player = (Player*) behave("ob_pplayer");
    float height = player->get_height();
    float width = player->get_width();
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+width, pp.y+height, color);
}

void BallPen::draw() {
    Point pp = parent->get_global_position();
    float radius = ((Ball*) behave("ob_pball"))->get_radius();
    al_draw_filled_circle(pp.x, pp.y, radius, color);
}

void TablePen::draw() {
    Point pp = parent->get_global_position();
    Point& bounds = ((Table*) behave("ob_ptable"))->bounds;
    al_draw_filled_rectangle(pp.x, pp.y, pp.x+bounds.x, pp.y+bounds.y, color);
}
