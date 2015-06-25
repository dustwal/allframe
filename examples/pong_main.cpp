#include "pong.h"

int main() {

    init();

    Game<Pong> game();
    game.run();

    close();

}
