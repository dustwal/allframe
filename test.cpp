#include <iostream>

#include "allframe.h"

using namespace allframe;

class RandomDraw : public Pen {

    public:
        void draw() const {
            std::cout << "nooo" << std::endl;
            al_clear_to_color(Colors::random_color());
        }

};

class TestState : public GameState {

    public:
        TestState(ALLEGRO_DISPLAY* display) : GameState(display) {}

    protected:
        void setup() {
            GameObject object(this, "o1");
            Pen* pen = new RandomDraw;
            object.set_pen(pen);
            add_object(object);
            std::cout << "setup " << get_object("o1") << " " << objects->size() << std::endl;
        }
};

int main() {
    if (init() != 0) return 1;
    std::cout << "allegro modules initialized" << std::endl;
    
    Game<TestState> game;
    game.run();
    close();
}
