#ifndef __GAME_INL_
#define __GAME_INL_

namespace allframe {

    template <typename T>
    class Game {
        
        public:

            // runs the game fullsreen
            inline Game() : display(NULL) {
                check_type(static_cast<T*>(0));
                ALLEGRO_DISPLAY_MODE disp_data;
                al_get_display_mode(/*al_get_num_display_modes()-1*/0, &disp_data);
                al_set_new_display_flags(ALLEGRO_FULLSCREEN);
                width = disp_data.width;
                height = disp_data.height;
            }

            inline Game(int width, int height) : height(height), width(width), display(NULL) {
                check_type(static_cast<T*>(0));
            }

            inline ~Game() {
                al_destroy_display(display);
            }

            inline void run() {
                display = al_create_display(width, height);
                GameState state = T(display);
                state.start();
            }

        private:

            int height;
            int width;
            ALLEGRO_DISPLAY*    display;

            inline void check_type(GameState*) {}
    };

};

#endif
