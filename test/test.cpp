#include <iostream>

#include "../core/allframe.h"

using namespace allframe;

class RandomDraw : public Pen {

    public:
        void draw() const {
            //std::cout << "nooo" << std::endl;
            al_clear_to_color(Colors::random_color());
            //Point pp = parent->get_position();
            //al_draw_filled_rectangle(pp.x,pp.y,pp.x+50,pp.y+50,al_map_rgb(0,155,155));
        }

};

class Gravity : public ObjectBehavior {

    public:
        void setup() {
            ddy = 1.0f;
            dy = 0;
        }

        void update() {
            Point pp = parent->get_position();
            if (pp.y+dy >= 500)
                dy = -dy;
            dy += ddy;
            pp.y += dy;
            parent->set_position(pp);
        }

    private:
        float ddy;
        float dy;
        std::string behavior_name = "gravity";
};

class DrawRectangle : public Pen {

    public:
        void draw() const {
            Point pp = parent->get_position();
            al_draw_filled_rectangle(pp.x,pp.y,pp.x+50,pp.y+50,al_map_rgb(0,155,155));
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
            name = add_object("o2");
            obj_ptr = get_object(name);
            if (obj_ptr != NULL) {
                pen = new DrawRectangle;
                ObjectBehavior* ob = new Gravity;
                obj_ptr->set_pen(pen);
                obj_ptr->add_behavior(ob);
            }
            name = add_object_topof("o3", "o1");
            obj_ptr = get_object(name);
            if (obj_ptr != NULL) {
                pen = new DrawRectangle;
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
    std::cout << "end run" << std::endl;
    close();
    std::cout << "close" << std::endl;
}
