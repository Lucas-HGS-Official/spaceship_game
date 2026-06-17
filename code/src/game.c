#include "game.h"

#include <raylib.h>

#include "defines.h"



void _update_game(void);
void _draw_game(void);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);

    return;
}
void game_loop(void) {
    while (!WindowShouldClose()) {
        _update_game();
        _draw_game();
    }

    return;
}
void game_close(void) {
    CloseAudioDevice();
    CloseWindow();

    return;
}


void _update_game(void) {

}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(RAYWHITE);

    EndDrawing();
    return;
}
