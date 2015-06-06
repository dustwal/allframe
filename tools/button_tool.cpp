#include "../core/allframe.h"

using namespace allframe;

class JoystickAxis : public EventHandler {
    public:
        void event(ALLEGRO_EVENT& event) {
            
            if (event.type == ALLEGRO_EVENT_JOYSTICK_AXIS) {
                std::cout<< "joystick axis: " << std::endl;
                std::cout<< "\tid: " << event.joystick.id << std::endl;
                std::cout<< "\tstick: " << event.joystick.stick << std::endl;
                std::cout<< "\taxis: " << event.joystick.axis << std::endl;
                std::cout<< "\tpos: " << event.joystick.pos << std::endl;
            } else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN) {
                std::cout << "joystick button down: " << std::endl;
                std::cout << "id: " << event.joystick.id << std::endl;
                std::cout << "button: " << event.joystick.button << std::endl;
            } else if (event.type == ALLEGRO_EVENT_JOYSTICK_BUTTON_UP) {
                std::cout << "joystick button up: " << std::endl;
                std::cout << "id: " << event.joystick.id << std::endl;
                std::cout << "button: " << event.joystick.button << std::endl;
            }
        }
};

class KeyboardHandle : public EventHandler {

    public:
        void event(ALLEGRO_EVENT& event) {
            if (event.type == ALLEGRO_EVENT_KEY_DOWN) {
                std::cout << "key down" << std::endl;
                std::cout << "key: " << event.keyboard.keycode << std::endl;
            }
        }
};

class VisualInput : public GameState {
    public:
        VisualInput(ALLEGRO_DISPLAY* display) : GameState(display) {}
        void setup() {
            auto& emap = *event_map;
            al_install_joystick();
            al_install_keyboard();
            al_register_event_source(event_queue, al_get_joystick_event_source());
            al_register_event_source(event_queue, al_get_keyboard_event_source());
            
            EventHandler* handles = new JoystickAxis;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_JOYSTICK_AXIS] = handles;
            emap[ALLEGRO_EVENT_JOYSTICK_BUTTON_DOWN] = handles;
            emap[ALLEGRO_EVENT_JOYSTICK_BUTTON_UP] = handles;
            handles = new KeyboardHandle;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_KEY_DOWN] = handles;
        }
};

int main() {
    init();

    Game<VisualInput> game(100,100);
    game.run();

    close();
}
