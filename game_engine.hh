#ifndef __GAME_ENGINE_HH__
#define __GAME_ENGINE_HH__

#include <allegro5/allegro.h>

namespace alengi {
    
    
    int init();
    int close();

    class GameObject {

    };

    class GameState {

    };

    class Game {

        public:

            // runs game fullscreen
            Game(GameState* gs_array, int size, int default_state);

            // runs game with the specified dimansions
            Game(GameState* gs_array, int size, int default_state, int width, int height);

            // destructor delete display
            ~Game();

            // runs the game
            void run();
        
        private:

            GameState*          game_state_array;
            int                 game_state_array_size;
            GameState           default_state;
            ALLEGRO_DISPLAY*    display;

    };


};

#endif
