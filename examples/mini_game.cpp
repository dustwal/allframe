#include "../core/allframe.h"

using namespace allframe;

class Box : public Pen {

    public:
        void draw() const {
            al_draw_filled_rectangle(20,20,505,505,al_map_rgb(10,15,20));
        }
};

class Circle : public Pen {

    public:
        void draw() const {
            Point pp = parent->get_position();
            al_draw_filled_circle(pp.x,pp.y,5,Colors::random_color());
        }
};

class Ball : public ObjectBehavior {
    
    public:
        std::string get_name() const { return "ball"; }

        void setup() {
            x = 250;
            y = 250;
            dx = 0;
            dy = 0;
        }

        void update() {
            x += dx;
            y += dy;
            if (x >= 500) {
                x = 500;
                dx = -dx;
            } else if (x <= 25) {
                x = 25;
                dx = -dx;
            }
            if (y >= 500) {
                y = 500;
                dy = -dy;
            } else if (y <= 25) {
                y = 25;
                dy = -dy;
            }
            parent->set_position({x,y});
        }

        void accelerate_left() {
            dx -= 1;
        }

        void accelerate_right() {
            dx += 1;
        }

        void accelerate_up() {
            dy -= 1;
        }

        void accelerate_down() {
            dy += 1;
        }

    private:
        double x;
        double y;
        double dx;
        double dy;

};

class KeyHandler : public EventHandler {
    public:
        KeyHandler(GameState* parent) : EventHandler(parent) {}
        void event(ALLEGRO_EVENT& event) {
            Ball* ball = (Ball*) parent->get_object("ball")->get_behavior("ball");
            switch (event.keyboard.keycode) {
                case ALLEGRO_KEY_ESCAPE:
                    parent->signal_close();
                    break;
                case ALLEGRO_KEY_W:
                    ball->accelerate_up();
                    break;
                case ALLEGRO_KEY_A:
                    ball->accelerate_left();
                    break;
                case ALLEGRO_KEY_S:
                    ball->accelerate_down();
                    break;
                case ALLEGRO_KEY_D:
                    ball->accelerate_right();
                    break;
            }
           
        }
};

class BallGame : public GameState {
    
    public:
        BallGame(ALLEGRO_DISPLAY* display) : GameState(display) {}
    protected:
        void setup() {
            auto& emap = *event_map;
            al_install_keyboard();
            al_register_event_source(event_queue, al_get_keyboard_event_source());
            emap[ALLEGRO_EVENT_KEY_DOWN] = new KeyHandler(this);

            // wall
            std::string name = add_object("wall");
            GameObject* obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new Box);
            }
            // ball
            name = add_object("ball");
            obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new Circle);
                obj->add_behavior(new Ball);
            }
        }
};

int main() {

    init();

    Game<BallGame> game;
    game.run();
    close();
    
}
