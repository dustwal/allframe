// Dustin Walde 2015
// pong_main.cpp

#include <allegro5/allegro.h>
#include <cstdlib>

#include "pong.h"

using namespace allframe;

int main(int argc, char** argv) {

    init();

    if (argc < 3) {
        Game<pong::Pong> game;
        game.run();
    } else {
        Game<pong::Pong> game(std::atoi(argv[1]), std::atoi(argv[2]));
        game.run();
    }

    close();

}
