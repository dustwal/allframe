#ifndef __PONG_H__
#define __PONG_H__

// pong.h
// Dustin Walde 2015

#include <map>
#include <set>

#include "../core/allframe.h"

using namespace allframe;

namespace pong {

    class Player;

    // Handles mapping of controller to player and sends movements
    // to the game
    class ControlController : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_contcont"; }

            void setup();
            void destroy();

            // int 1 -> wasd -> 2 arrows, joystick.id
            void map_player(uint64_t id, Player* player);
            // called when the new velocity is known
            // 1 up 0 down
            void action_axis(uint64_t id, float val);
            // called on button up / button down
            void action_button(uint64_t id, bool movement, bool press);


        private:
            // map 
            std::map<uint64_t, Player*>* player_map;
            std::map<uint64_t, bool*>* pressed;
    };

    class PongLogic : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_ponglogic"; }

            void setup();
            void update();
            void destroy();
            void action(uint64_t);

        private:

            void ball_reset();
            void game_over();
            Player* new_player();
            void start_ball();

            int num_players;
            int registered;
            std::set<uint64_t>* ids;
            int* scores;
            uint8_t last_score;
            bool ball_ready;

    };

    // ALLEGRO_EVENT_JOYSTICK_AXIS
    // ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN
    class JoystickController : public EventHandler {
        public:
            void event(ALLEGRO_EVENT&);
    };

    // ALLEGRO_EVENT_KEY_DOWN
    // ALLEGRO_EVENT_KEY_UP
    class KeyboardController : public EventHandler {
        public:
            void event(ALLEGRO_EVENT&);
    };

    class Player : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_pplayer"; }

            void setup();
            void update();
            void destroy();
            // sets the velocity val between -1 and 1
            // checks the bounds of the float
            void set_velocity(float);
            float get_height();
            float get_width();

            const float player_wid = .05f;
            const float player_hig = .2f;
            const float ppixel_speed = 20.0f;

            Bounds* bounds;

        private:
            float velocity;
            
            Point parent_bounds;

            float pix_hig;
            float pix_wid;

    };

    // Draws the player pad
    // player anchor is on top left
    class PlayerPen : public Pen {

        public:
            ALLEGRO_COLOR color = al_map_rgb(10,240,240);
            void draw();

    };

    class Ball : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_pball"; }

            void setup();
            void update();
            float get_radius();

            Point velocity;
            const double max_v = 70;

        private:
            float radius;
            Point parent_bounds;
            float pix_rad;
    };

    class BallPen : public Pen {
        public:
            ALLEGRO_COLOR color = al_map_rgb(255,10,10);
            void draw();

    };

    class Table : public ObjectBehavior {
        public:
            std::string get_name() const { return "ob_ptable"; }
            void setup();
            Point bounds;
    };

    class TablePen : public Pen {
        public:
            ALLEGRO_COLOR color = al_map_rgb(10,10,10);
            void draw();
    };

    // TODO
    class Pong : public GameState {

        public:
            Pong(ALLEGRO_DISPLAY* display) : GameState(display) {}
        protected:
            // TODO
            void setup();
            // TODO
            void destroy();
    };

};

#endif
