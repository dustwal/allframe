#include "../core/allframe.h"

using namespace allframe;

class JoystickAxis : public EventHandler {
    public:
        void event(ALLEGRO_EVENT& event) {
            std::cout<< "joystick axis: " << std::endl;
            std::cout<< "\tid: " << event.joystick.id << std::endl;
            std::cout<< "\tstick: " << event.joystick.stick << std::endl;
            std::cout<< "\taxis: " << event.joystick.axis << std::endl;
            std::cout<< "\tpos: " << event.joystick.pos << std::endl;
        }
};

class VisualInput : public GameState {
    public:
        VisualInput(ALLEGRO_DISPLAY* display) : GameState(display) {}
        void setup() {
            auto& emap = *event_map;
            al_install_joystick();
            al_register_event_source(event_queue, al_get_joystick_event_source());
            
            EventHandler* handles = new JoystickAxis;
            handles->set_parent_state(this);
            emap[ALLEGRO_EVENT_JOYSTICK_AXIS] = handles;
        }
};

int main() {
    init();

    Game<VisualInput> game(100,100);
    game.run();

    close();
}
