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

};

// template <GameState> class Game 
#include "game.inl"

#endif
