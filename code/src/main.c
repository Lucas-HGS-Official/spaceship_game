#include "game.h"

int main(void) {
    game_init();
    game_loop();
    game_close();

    return 0;
}
