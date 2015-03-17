#include <iostream>

#include "game_frame.h"

using namespace allframe;

int main() {
    if (init() != 0) return 1;
    std::cout << "allegro modules initialized" << std::endl;

    Game<GameState> game;
    game.run();
    close();
}
