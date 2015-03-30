#include <cmath>

#include "../core/allframe.h"

using namespace allframe;

class Pong;
class Menu;

GameState* create_menu(ALLEGRO_DISPLAY*);

class Ball : public ObjectBehavior {
    public:
        void update() {
        
        }

        std::string get_name() const { return "ball"; }
};

class Player : public ObjectBehavior {
    public:
        Player() : bounds(new BoxBounds({50,50})), dir(0) {}
        ~Player() { delete bounds; }

        void update() {
            Point pp = parent->get_global_position();
            pp.y = std::max(std::min(pp.y+10*dir, 450.0), 0.0);
            parent->set_global_position(pp);
        }

        void set_dir(float val) {
            dir += val;
        }

        void set_vel(float val) {
            dir = val;
        }

        std::string get_name() const { return "player"; }
    private:
        Bounds* bounds;
        float dir;
};

class PlayerPen : public Pen {
    public:
        PlayerPen(bool first) : is_p1(first) {}
        void draw() const {
            Point pp = parent->get_global_position();
            if (is_p1) pp.x += 40;
            al_draw_filled_rectangle(pp.x, pp.y, pp.x+10, pp.y+50, al_map_rgb(0,255,0));
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
                obj->set_pen(new PlayerPen(true));
                obj->add_behavior(new Player);
                obj->set_position({0, 200});
            }
            name = add_object("player2");
            obj = get_object(name);
            if (obj != NULL) {
                obj->set_pen(new PlayerPen(false));
                obj->add_behavior(new Player);
                obj->set_position({450, 200});
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
