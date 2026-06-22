#include "game.h"

#include <raylib.h>
#include <raymath.h>
#include <stdio.h>

#include "defines.h"

#define STARS_NUM 20

typedef struct Sprite {
    Texture2D texture;
    Rectangle src_rec;
    Rectangle dest_rec;
    Vector2 origin;
} Sprite;

static Sprite player_sprite = {};
static Vector2 player_direction = {};
static float player_speed = 0.f;

static Sprite meteor_sprite = {};

static Sprite laser_sprite = {};

static Texture2D star_texture = {};
static Vector2 star_coords_array[STARS_NUM] = {};


void _update_game(float dt);
void _draw_game(void);

Vector2 _gen_rand_coords(void);
void _draw_stars(void);
void _init_sprite(Sprite* sprite, char* texture_file_path);
void _draw_sprite(Sprite* sprite);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);

    _init_sprite(&player_sprite, "resources/images/player.png");
    player_direction = (Vector2) {};
    player_direction = Vector2Normalize(player_direction);
    player_speed = 400.f;

    _init_sprite(&meteor_sprite, "resources/images/meteor.png");

    _init_sprite(&laser_sprite, "resources/images/laser.png");
    laser_sprite.origin = (Vector2) { .x=laser_sprite.src_rec.width+20, .y=laser_sprite.src_rec.height+20 };
    laser_sprite.dest_rec.x = WINDOW_WIDTH; laser_sprite.dest_rec.y = WINDOW_HEIGHT;

    star_texture = LoadTexture("resources/images/star.png");

    for (int i=0; i<STARS_NUM; i++) {
        star_coords_array[i] = _gen_rand_coords();
    }

    return;
}
void game_loop(void) {
    while (!WindowShouldClose()) {
        _update_game(GetFrameTime());
        _draw_game();
    }

    return;
}
void game_close(void) {
    CloseAudioDevice();
    CloseWindow();

    UnloadTexture(player_sprite.texture);
    UnloadTexture(star_texture);

    return;
}


void _update_game(float dt) {
    player_direction = (Vector2){ (int)IsKeyDown(KEY_RIGHT) - (int)IsKeyDown(KEY_LEFT), IsKeyDown(KEY_DOWN) - (int)IsKeyDown(KEY_UP) };
    player_direction = Vector2Normalize(player_direction);
    if (IsKeyPressed(KEY_SPACE)) { printf("\nfire laser\n"); }

    player_sprite.dest_rec.x += player_direction.x * player_speed * dt;
    player_sprite.dest_rec.y += player_direction.y * player_speed * dt;

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    _draw_stars();
    _draw_sprite(&meteor_sprite);
    _draw_sprite(&player_sprite);
    _draw_sprite(&laser_sprite);

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

void _init_sprite(Sprite* sprite, char* texture_file_path) {
    sprite->texture = LoadTexture(texture_file_path);
    sprite->src_rec = (Rectangle) {
        .width = sprite->texture.width, .height = sprite->texture.height,
        .x = 0, .y = 0,
    };
    sprite->dest_rec = (Rectangle) {
        .height = sprite->src_rec.height, .width = sprite->src_rec.width,
        .x = WINDOW_WIDTH/2.f, .y = WINDOW_HEIGHT/2.f, // TODO
    };
    sprite->origin = (Vector2) { .x = sprite->src_rec.width/2.f, .y = sprite->src_rec.height/2.f };

    return;
}

void _draw_sprite(Sprite* sprite) {
    DrawTexturePro(sprite->texture, sprite->src_rec, sprite->dest_rec, sprite->origin, 0.f, WHITE);

    return;
}
