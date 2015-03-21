#include <iostream>

#include "allframe.h"

using namespace allframe;

class RandomDraw : public Pen {

    public:
        void draw() const {
            //std::cout << "nooo" << std::endl;
            al_clear_to_color(Colors::random_color());
        }

};

class TestState : public GameState {

    public:
        TestState(ALLEGRO_DISPLAY* display) : GameState(display) {}

    protected:
        void setup() {
            Pen* pen = new RandomDraw; // TODO never freed
            std::string name = add_object("o1");
            GameObject* obj_ptr = get_object(name);
            if (obj_ptr != NULL) {
                obj_ptr->set_pen(pen);
            }
            std::cout << "setup " << name << " " << objects->size() << std::endl;
        }
};

int main() {
    if (init() != 0) return 1;
    std::cout << "allegro modules initialized" << std::endl;
    
    Game<TestState> game;
    game.run();
    close();
}
