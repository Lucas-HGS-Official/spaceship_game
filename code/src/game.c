#include "game.h"

#include <stdbool.h>

#include <raylib.h>
#include <raymath.h>

#include "defines.h"

#define STARS_NUM 20

#define LASER_COOLDOWN .2f

#define MAX_LASERS 30
#define LASER_SPEED 1400

#define MAX_METEORS 10
#define METEOR_COOLDOWN 0.3f
#define METEOR_MAX_SPEED 500
#define METEOR_MIN_SPEED 300

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
    Texture2D alpha_mask;
} Player;

typedef struct Meteor {
    bool is_in_use;
    Sprite spr;
    Vector2 direction;
    float speed;
} Meteor;

typedef struct Laser {
    bool is_in_use;
    Sprite spr;
    float speed;
} Laser;


static bool is_game_running = true;
static Font custom_font = {};
static int current_score = 0;

static Player player = {0};

static Sprite meteor_sprite = {0};
static Meteor meteor_list[MAX_METEORS] = {0};
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
void _instance_meteor(Vector2 position);
void _draw_all_meteors(void);
void _update_all_meteors(float dt);

void _instance_laser(Vector2 position);
void _draw_all_lasers(void);
void _update_all_laser(float dt);

void _all_collisions(void);
void _draw_game_over_screen(void);
void _draw_score(void);


void game_init(void) {
    InitWindow(WINDOW_WIDTH, WINDOW_HEIGHT, GAME_NAME);
    InitAudioDevice();
    SetTargetFPS(60);
    is_game_running = true;
    custom_font = LoadFont("resources/images/Oxanium-Bold.ttf");
    SetTextureFilter(custom_font.texture, TEXTURE_FILTER_BILINEAR);

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

    UnloadTexture(*(laser_sprite.texture));
    MemFree(laser_sprite.texture);

    return;
}


void _update_game(float dt) {
    if (is_game_running) {
        current_score = GetTime();
        _update_player(dt);
        _meteor_cooldown_timer(METEOR_COOLDOWN);
        _update_all_laser(dt);
        _update_all_meteors(dt);
        _all_collisions();
    }

    return;
}
void _draw_game(void) {
    BeginDrawing();
    if (is_game_running) {
        ClearBackground((Color) { 58, 46, 63, 255 });

        _draw_stars();
        _draw_all_lasers();
        _draw_all_meteors();
        _draw_sprite(&player.spr);
        _draw_score();

    } else if (!is_game_running) {
        _draw_game_over_screen();
    }

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

    Image player_image = LoadImageFromTexture(*(player->spr.texture));
    Image player_alpha_image = ImageFromChannel(player_image, 3);
    player->alpha_mask = LoadTextureFromImage(player_alpha_image);


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
        float half_meteor_width = meteor_sprite.texture->width/2.f;
        Vector2 meteor_rand_coords = {
            .x = GetRandomValue(half_meteor_width, WINDOW_WIDTH-half_meteor_width),
            .y = GetRandomValue(-half_meteor_width*2, -half_meteor_width*4)
        };
        _instance_meteor(meteor_rand_coords);
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
            laser_list[i].speed = LASER_SPEED;

            laser_list[i].is_in_use = true;
        } else { is_not_empty_slot = true; }
    }

    return;
}
void _draw_all_lasers(void) {
    for (int i=0; i<MAX_LASERS; i++) {
        if (laser_list[i].is_in_use) {
            _draw_sprite(&laser_list[i].spr);
        }
    }

    return;
}
void _update_all_laser(float dt) {
    for (int i=0; i<MAX_LASERS; i++) {
        if (laser_list[i].is_in_use) {
            laser_list[i].spr.dest_rec.y -= laser_list[i].speed * dt;
            if (laser_list[i].spr.dest_rec.y <= -laser_sprite.dest_rec.height/2.f) { laser_list[i].is_in_use = false; }
        }
    }

    return;
}

void _instance_meteor(Vector2 position) {
    bool is_not_empty_slot = true;
    int i=0;
    for (i=0; i<MAX_METEORS && is_not_empty_slot; i++) {
        if (!meteor_list[i].is_in_use) {
            is_not_empty_slot = false;
            meteor_list[i] = (Meteor) {
                .spr = meteor_sprite,
            };
            meteor_list[i].spr.dest_rec.x = position.x;
            meteor_list[i].spr.dest_rec.y = position.y;
            meteor_list[i].speed = GetRandomValue(METEOR_MAX_SPEED, METEOR_MIN_SPEED);
            meteor_list[i].direction = Vector2Normalize((Vector2) { .x=(GetRandomValue(-5, 5)/10.f), .y=1 });

            meteor_list[i].is_in_use = true;
        } else { is_not_empty_slot = true; }
    }

    return;
}
void _draw_all_meteors(void) {
    for (int i=0; i<MAX_METEORS; i++) {
        if (meteor_list[i].is_in_use) {
            _draw_sprite(&meteor_list[i].spr);
        }
    }

    return;
}
void _update_all_meteors(float dt) {
    for (int i=0; i<MAX_METEORS; i++) {
        if (meteor_list[i].is_in_use) {
            meteor_list[i].spr.dest_rec.x += meteor_list[i].direction.x * meteor_list[i].speed * dt;
            meteor_list[i].spr.dest_rec.y += meteor_list[i].direction.y * meteor_list[i].speed * dt;
            if (meteor_list[i].spr.dest_rec.y >= WINDOW_HEIGHT+meteor_sprite.dest_rec.height/2.f) { meteor_list[i].is_in_use = false; }
        }
    }

    return;
}

void _all_collisions(void) {
    for (int i=0; i<MAX_METEORS; i++) {
        if (meteor_list[i].is_in_use) {
            if (CheckCollisionRecs(meteor_list[i].spr.dest_rec, player.spr.dest_rec)) {
                is_game_running = false;
            }
            for (int j = 0; j<MAX_LASERS; j++) {
                if (
                    laser_list[j].is_in_use &&
                    CheckCollisionRecs(meteor_list[i].spr.dest_rec, laser_list[j].spr.dest_rec)
                ) {
                    laser_list[j].is_in_use = false;
                    meteor_list[i].is_in_use = false;
                }
            }
        }
    }

    return;
}

void _draw_game_over_screen(void) {
    ClearBackground(DARKGRAY);

    _draw_stars();
    _draw_all_lasers();
    _draw_all_meteors();
    _draw_score();


    float font_padding = 20.f;

    const char* game_over_text = "Game Over!!";
    float font_size = 60.f;
    Vector2 game_over_size = MeasureTextEx(custom_font, game_over_text, font_size, 0.f);
    Vector2 font_position = {
        .x = WINDOW_WIDTH - game_over_size.x - font_padding,
        .y = font_padding,
    };

    DrawTextPro(custom_font, game_over_text, font_position, (Vector2) { 0, 0 }, 0.f, font_size, 0.f, WHITE);

    return;
}

void _draw_score(void) {
    float font_padding = 20.f;

    const char* score_text = TextFormat("Score: %i", current_score);
    float font_size = 30.f;
    float font_spacing = 0.f;
    float font_rotation = 0.f;
    Vector2 font_position = {
        .x = font_padding,
        .y = font_padding,
    };
    Vector2 score_size = MeasureTextEx(custom_font, score_text, font_size, 0.f);
    Rectangle rounded_rec = {
        .width = score_size.x + 10,
        .height = score_size.y + 10,
        .x = font_position.x - 5,
        .y = font_position.y - 5,
    };
    DrawRectangleRoundedLines(rounded_rec, .5, 0, WHITE);

    DrawTextPro(custom_font, score_text, font_position, (Vector2) { 0, 0 }, font_rotation, font_size, font_spacing, WHITE);

    return;
}
