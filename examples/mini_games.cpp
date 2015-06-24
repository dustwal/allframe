#include <cmath>

#include "../core/allframe.h"

using namespace allframe;

class Pong;
class Menu;

int PLAYER_SIZE = 100;

GameState* create_menu(ALLEGRO_DISPLAY*);

class BallPen : public Pen {
    public:
        void draw() const {
            Point pp = parent->get_global_position();
            al_draw_filled_circle(pp.x, pp.y, 5, al_map_rgb(255,255,0));
        }
};

class Player : public ObjectBehavior {
    public:
        Player() : dir(0) {}

        void update() {
            Point pp = parent->get_global_position();
            pp.y = std::max(std::min(pp.y+15*dir, 500.0-PLAYER_SIZE), 0.0);
            parent->set_global_position(pp);
        }

        void set_dir(float val) {
            dir += val;
        }

        void set_vel(float val) {
            dir = val;
        }

        bool collision(Point& pold, Point& pnew) {
            Point pp = parent->get_global_position();
            if (pold.x < pnew.x) {
                if (pp.x < pold.x || pp.x > pnew.x) return false;
            } else {
                if (pp.x > pold.x || pp.x < pnew.x) return false;
            }
            float yval = (((pp.x+5)/pold.x)/(pnew.x-pold.x))*(pnew.y-pold.y)+pold.y;
            return yval >= pp.y && yval <= pp.y+PLAYER_SIZE;
        }

        std::string get_name() const { return "player"; }
    private:
        float dir;
};

class Ball : public ObjectBehavior {
    public:
        void setup() {
            velocity = {-5,0};
        }
        void update() {
            Point pp = parent->get_global_position();
            Point ppold = pp;
            pp.x += velocity.x;
            pp.y += velocity.y;
            if (pp.y <= 0) { 
                pp.y = 0;
                velocity.y = -velocity.y;
            } else if (pp.y >= 500) {
                pp.y = 500;
                velocity.y = -velocity.y;
            }
            Player* p1 = (Player*) parent_state->get_object("player1")->get_behavior("player");
            Player* p2 = (Player*) parent_state->get_object("player2")->get_behavior("player");
            if (p1->collision(ppold, pp)) {
                pp.x = 55;
                velocity.x *= -1.15;
            } else if (p2->collision(ppold, pp)) {
                pp.x = 445;
                velocity.x *= -1.15;
            }
            if (pp.x <= 0 || pp.x >= 500) {
                pp.x = 250;
                pp.y = 250;
                velocity.x = 2;
                velocity.y = 0;
            }
            parent->set_global_position(pp);
        }

        std::string get_name() const { return "ball"; }
    private:
        Point velocity;

};

class PlayerPen : public Pen {
    public:
        void draw() const {
            Point pp = parent->get_global_position();
            al_draw_filled_rectangle(pp.x, pp.y, pp.x+10, pp.y+PLAYER_SIZE, al_map_rgb(0,255,0));
        }
    private:
        bool is_p1;
};

class KeyHandler : public EventHandler {
    public:
        void event(ALLEGRO_EVENT& event) {
            Player* p1 = (Player*) parent->get_object("player1")->get_behavior("player");
            Player* p2 = (Player*) parent->get_object("player2")->get_behavior("player");
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                switch (event.keyboard.keycode) {
                    case ALLEGRO_KEY_ESCAPE:
                        parent->signal_next_state(create_menu(parent->get_display()));
                        break;
                    case ALLEGRO_KEY_W:
                        p1->set_dir(-1);
                        break;
                    case ALLEGRO_KEY_S:
                        p1->set_dir(1);
                        break;
                    case ALLEGRO_KEY_UP:
                        p2->set_dir(-1);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        p2->set_dir(1);
                        break;
                }
            } else {
                switch(event.keyboard.keycode) {
                    case ALLEGRO_KEY_W:
                        p1->set_dir(1);
                        break;
                    case ALLEGRO_KEY_S:
                        p1->set_dir(-1);
                        break;
                    case ALLEGRO_KEY_UP:
                        p2->set_dir(1);
                        break;
                    case ALLEGRO_KEY_DOWN:
                        p2->set_dir(-1);
                        break;
                }
            }
        }
};

class JoystickMove : public EventHandler {

    public:
        void event(ALLEGRO_EVENT& event) {
            Player* p1 = (Player*) parent->get_object("player1")->get_behavior("player");
            Player* p2 = (Player*) parent->get_object("player2")->get_behavior("player");
            if (event.joystick.axis == 1) {
                p1->set_vel(event.joystick.pos);    
            }
        }
};

class Menu : public GameState {
    public: 
        Menu(ALLEGRO_DISPLAY* display) : GameState(display) {}
        void setup() {
            auto& emap = *event_map;
            std::cout << "mouse install" << std::endl;
            al_install_mouse();
            al_register_event_source(event_queue, al_get_mouse_event_source());
            
            // event handlers
            EventHandler* handles = new ClickEvent;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_MOUSE_BUTTON_DOWN] = handles;
            emap[ALLEGRO_EVENT_MOUSE_BUTTON_UP] = handles;

            std::string name = add_object("pong");
            GameObject* obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new ButtonPen);
                Clickable* button = new StateMenuButton<Pong>;
                button->set_bounds(new BoxBounds({100,50}));
                obj->add_behavior(button);
                obj->set_position({200,200});
            }
        }

        void destroy() {
            std::cout << "mouse uninstall" << std::endl;
            al_uninstall_mouse();
        }
};

class Pong : public GameState {
    public:
        Pong(ALLEGRO_DISPLAY* display) : GameState(display) {}
        void setup() {
            auto& emap = *event_map;
            al_install_joystick();
            al_install_keyboard();
            //al_install_mouse();
            std::cout << "keyboard & mouse install" << std::endl;
            al_register_event_source(event_queue, al_get_joystick_event_source());
            al_register_event_source(event_queue, al_get_keyboard_event_source());
            //al_register_event_source(event_queue, al_get_mouse_event_source());

            // event handlers
            EventHandler* handles = new ClickEvent;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_MOUSE_BUTTON_DOWN] = handles;
            emap[ALLEGRO_EVENT_MOUSE_BUTTON_UP] = handles;
            handles = new KeyHandler;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_KEY_DOWN] = handles;
            emap[ALLEGRO_EVENT_KEY_UP] = handles;
            handles = new JoystickMove;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_JOYSTICK_AXIS] = handles;

            std::string name = add_object("player1");
            GameObject* obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new PlayerPen);
                obj->add_behavior(new Player);
                obj->set_position({40, 200});
            }
            name = add_object("player2");
            obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new PlayerPen);
                obj->add_behavior(new Player);
                obj->set_position({450, 200});
            }
            name = add_object("ball");
            obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new BallPen);
                obj->add_behavior(new Ball);
                obj->set_position({250,250});
            }
        }

        void destroy() {
            al_uninstall_keyboard();
        }
};

GameState* create_menu(ALLEGRO_DISPLAY* disp) { return new Menu(disp); }

int main() {
    init();

    Game<Menu> game(500,500);
    game.run();
    close();
}
