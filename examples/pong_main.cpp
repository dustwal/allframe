#include "pong.h"

using namespace allframe;

int main() {

    init();

    Game<pong::Pong> game;
    game.run();

    close();

}
