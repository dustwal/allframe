#include "../core/allframe.h"
namespace pong {

    const player_wid_rat = .05;

    int player_width;

    // TODO
    class ControlController : public ObjectBehavior {

        public:
            std::string get_name() const { return "ob_contcont"; }

            // TODO
            void setup();
            // TODO
            void update();
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
    class JoystickController : public EventHandler {

        public:
            // TODO
            void event();
    };

    // TODO
    class KeyboardController : public EventHandler {

            // TODO
    };

    // TODO
    class Player : public ObjectBehavior {

        public:
            std::string get_name() const ( return "ob_pplayer"; }

            // TODO
            void setup();
            // TODO
            void update();
    };

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
