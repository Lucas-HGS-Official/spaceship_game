#include "game.h"

#include <raylib.h>

#include "defines.h"

#define STARS_NUM 20

static Texture2D player_texture = (Texture2D) {};
static Texture2D star_texture = (Texture2D) {};
static Vector2 star_coords_array[STARS_NUM] = {};


void _update_game(void);
void _draw_game(void);

Vector2 _gen_rand_coords(void);
void _draw_stars(void);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);

    player_texture = LoadTexture("resources/images/player.png");

    star_texture = LoadTexture("resources/images/star.png");

    for (int i=0; i<STARS_NUM; i++) {
        star_coords_array[i] = _gen_rand_coords();
    }

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

    UnloadTexture(player_texture);
    UnloadTexture(star_texture);

    return;
}


void _update_game(void) {

}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(DARKGRAY);
    static float x = 50.f;
    x++;

    _draw_stars();
    DrawTextureEx(player_texture, (Vector2) { .x=x, .y=150 }, 0.f, 1.f, WHITE);

    EndDrawing();

    return;
}


Vector2 _gen_rand_coords(void) {
    Vector2 rand_coords = {};

    float x_coords = GetRandomValue(star_texture.width, WINDOW_WIDTH-star_texture.width);
    float y_coords = GetRandomValue(star_texture.height, WINDOW_HEIGHT-star_texture.height);
    rand_coords = (Vector2) { .x=x_coords, .y=y_coords };

    return rand_coords;
}

void _draw_stars(void) {
    for (int i=0; i<STARS_NUM; i++) {
        DrawTextureEx(star_texture, star_coords_array[i], 0.f, 1.f, WHITE);
    }

    return;
}
