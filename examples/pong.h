#include "../core/allframe.h"
namespace pong {

    const player_wid_rat = .05;

    int player_width;

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
            void action_button(uint64_t id, bool movement, bool press)


        private:
            // map 
            std::map<uint64_t, Player*>* player_map;
            std::map<uint64_t, bool*>* pressed;
    };

    // TODO
    class PongLogic : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_ponglogic"; }

            // TODO
            void setup();
            // TODO
            void update();
    };

    // TODO
    // ALLEGRO_EVENT_JOYSTICK_AXIS
    // ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN
    // ALLEGRO_EVENT_JOYSTICK_BUTTON_UP
    class JoystickController : public EventHandler {

        public:
            // TODO
            void event(ALLEGRO_EVENT&);
    };

    // TODO
    // ALLEGRO_EVENT_KEY_DOWN
    // ALLEGRO_EVENT_KEY_UP
    class KeyboardController : public EventHandler {

            // TODO
            void event(ALLEGRO_EVENT&);
    };

    // TODO
    class Player : public ObjectBehavior {

        public:
            std::string get_name() const ( return "ob_pplayer"; }

            // TODO
            void setup();
            // TODO
            void update();
            // TODO val between -1 and 1
            void set_velocity(float);
    };

    // Draws the player pad
    // player anchor is on top left
    class PlayerPen : public Pen {

        public:
            ALLEGRO_COLOR color = al_map_rgb(10,240,240);
            void draw();

    };

    // TODO
    class Ball : public ObjectBehavior {

        public:
            std::string get_name() const ( return "ob_pball"; }

            // TODO
            void setup();
            // TODO
            void update();
    };

    class BallPen : public Pen {

        public:
            ALLEGRO_COLOR color = al_map_rgb(255,10,10);
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
