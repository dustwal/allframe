#ifndef __CORE_BEHAVIOR_H_
#define __CORE_BEHAVIOR_H_

#include "game_frame.h"

namespace allframe {

    namespace Colors {

        const ALLEGRO_COLOR BLACK   = al_map_rgb(0,0,0);
        const ALLEGRO_COLOR WHITE   = al_map_rgb(255,255,255);

        const ALLEGRO_COLOR RED     = al_map_rgb(255,0,0);
        const ALLEGRO_COLOR GREEN   = al_map_rgb(0,255,0);
        const ALLEGRO_COLOR BLUE    = al_map_rgb(0,0,255);

        ALLEGRO_COLOR random_color();
        ALLEGRO_COLOR random_color_alpha();

    };

    class Clickable : public ObjectBehavior {

        public:
            

            void click_event();

        protected:
            bool    is_box;
            Point   bounds;
            virtual void on_click() {}


    };

    class MenuButton : public GameObject {
        
    };

    class GameMenu : public GameState {

    };

};

#endif
