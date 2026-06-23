#include "game.h"

#include <stdbool.h>

#include <raylib.h>
#include <raymath.h>

#include "defines.h"

#define STARS_NUM 20
#define MAX_LASERS 20
#define LASER_COOLDOWN .5f

typedef struct Sprite {
    Texture2D* texture;
    Rectangle src_rec;
    Rectangle dest_rec;
    Vector2 origin;
} Sprite;

typedef struct Player {
    Sprite spr;
    Vector2 direction;
    float speed;
    double laser_start_time;
    double laser_cooldown;
    bool is_shot_ready;
} Player;

typedef struct Laser {
    bool is_in_use;
    Sprite spr;
    Vector2 direction;
    float speed;
} Laser;

static Player player = {0};

static Sprite meteor_sprite = {0};
static double meteor_start_time = 0;

static Sprite laser_sprite = {0};
static Laser laser_list[MAX_LASERS] = {0};

static Texture2D star_texture = {0};
static Vector2 star_coords_array[STARS_NUM] = {0};


void _update_game(float dt);
void _draw_game(void);

Vector2 _gen_rand_coords(void);
void _draw_stars(void);

void _init_sprite(Sprite* sprite, char* texture_file_path);
void _draw_sprite(Sprite* sprite);

void _init_player(Player* player, double laser_cooldown);
void _update_player(float dt);

void _meteor_cooldown_timer(double cooldown);

void _instance_laser(Vector2 position);
void _draw_all_lasers(void);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);

    _init_player(&player, LASER_COOLDOWN);

    _init_sprite(&meteor_sprite, "resources/images/meteor.png");
    meteor_start_time = GetTime();

    _init_sprite(&laser_sprite, "resources/images/laser.png");

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

    UnloadTexture(*(player.spr.texture));
    MemFree(player.spr.texture);

    UnloadTexture(star_texture);

    UnloadTexture(*(meteor_sprite.texture));
    MemFree(meteor_sprite.texture);

    return;
}


void _update_game(float dt) {
    _update_player(dt);
    _meteor_cooldown_timer(0.5);

    return;
}
void _draw_game(void) {
    BeginDrawing();
    ClearBackground(DARKGRAY);

    _draw_stars();
    _draw_sprite(&meteor_sprite);
    _draw_all_lasers();
    _draw_sprite(&player.spr);

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
    sprite->texture = MemAlloc(sizeof(Texture2D));
    *(sprite->texture) = LoadTexture(texture_file_path);
    sprite->src_rec = (Rectangle) {
        .width = sprite->texture->width, .height = sprite->texture->height,
        .x = 0, .y = 0,
    };
    sprite->dest_rec = (Rectangle) {
        .height = sprite->src_rec.height, .width = sprite->src_rec.width,
        .x = WINDOW_WIDTH/2.f, .y = WINDOW_HEIGHT/2.f,
    };
    sprite->origin = (Vector2) { .x = sprite->src_rec.width/2.f, .y = sprite->src_rec.height/2.f };

    return;
}

void _draw_sprite(Sprite* sprite) {
    DrawTexturePro(*(sprite->texture), sprite->src_rec, sprite->dest_rec, sprite->origin, 0.f, WHITE);

    return;
}

void _init_player(Player* player, double laser_cooldown) {
    _init_sprite(&(player->spr), "resources/images/player.png");
    player->direction = (Vector2) {};
    player->direction = Vector2Normalize(player->direction);
    player->speed = 400.f;
    player->laser_start_time = GetTime();
    player->laser_cooldown = laser_cooldown;
    player->is_shot_ready = true;

    return;
}

void _update_player(float dt) {
    player.direction = (Vector2){
        .x = (int)IsKeyDown(KEY_RIGHT) - (int)IsKeyDown(KEY_LEFT),
        .y =  IsKeyDown(KEY_DOWN) - (int)IsKeyDown(KEY_UP)
    };
    player.direction = Vector2Normalize(player.direction);
    if (player.is_shot_ready) {
        if (IsKeyPressed(KEY_SPACE)) {
            _instance_laser((Vector2){ player.spr.dest_rec.x, player.spr.dest_rec.y });
            player.is_shot_ready = false;
            player.laser_start_time = GetTime();
        }
    }
    else if (player.laser_start_time + player.laser_cooldown <= GetTime()) {
        player.is_shot_ready = true;
        player.laser_start_time = GetTime();
    }

    Vector2 position = {
        .x = player.spr.dest_rec.x + (player.direction.x * player.speed * dt),
        .y = player.spr.dest_rec.y + (player.direction.y * player.speed * dt),
    };
    Vector2 half_size = {
        .x = player.spr.dest_rec.width/2.f,
        .y = player.spr.dest_rec.height/2.f,
    };
    player.spr.dest_rec.x = Clamp(position.x, half_size.x, WINDOW_WIDTH - half_size.x);
    player.spr.dest_rec.y = Clamp(position.y, half_size.y, WINDOW_HEIGHT - half_size.y);

    return;
}

void _meteor_cooldown_timer(double cooldown) {
    if (meteor_start_time + cooldown <= GetTime()) {
        // printf("\nCreate Meteor\n");
        meteor_start_time = GetTime();
    }

    return;
}

void _instance_laser(Vector2 position) {
    bool is_not_empty_slot = true;
    int i=0;
    for (i=0; i<MAX_LASERS && is_not_empty_slot; i++) {
        if (!laser_list[i].is_in_use) {
            is_not_empty_slot = false;
            laser_list[i] = (Laser) {
                .spr = laser_sprite,
            };
            laser_list[i].spr.dest_rec.x = position.x;
            laser_list[i].spr.dest_rec.y = position.y;
            laser_list[i].is_in_use = true;
        } else { is_not_empty_slot = true; }
    }

    return;
}

void _draw_all_lasers(void) {
    for (int i=0; i<MAX_LASERS && laser_list[i].is_in_use; i++) {
        _draw_sprite(&laser_list[i].spr);
        if (laser_list->spr.dest_rec.y <= -50) { laser_list[i].is_in_use = false; }
    }

    return;
}
