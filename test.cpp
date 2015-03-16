#include <iostream>

#include "game_engine.hh"

using namespace alengi;

int main() {
    if (init() != 0) return 1;
    std::cout << "allegro modules initialized" << std::endl;

    Game<GameState> game;
    game.run();
    close();
}
