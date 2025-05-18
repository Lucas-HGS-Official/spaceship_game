#include "settings.h"

typedef struct Entity {
    Texture2D texture;
    Rectangle src_rect;
    Rectangle dest_rect;
    Vector2 origin;
} entity_t;

typedef struct Sprite {
    Texture2D* texture;
    Rectangle src_rect;
} sprite_t;

typedef struct Position {
    Rectangle dest_rect;
    Vector2 origin;
} pos2_t;


void game_setup(entity_t* entities, ecs_world_t* world_flecs);
void game_loop(entity_t* entities, ecs_world_t* world_flecs);
void game_destroy(entity_t* entities, ecs_world_t* world_flecs);

void player_controls(entity_t* entities, float delta_time);

void game_render(entity_t* entities);

int main(void) {
    entity_t entities[MAX_NUM_ENTITIES];
    ecs_world_t* world_flecs = ecs_init();

    game_setup(entities, world_flecs);
    game_loop(entities, world_flecs);
    game_destroy(entities, world_flecs);

    return 0;
}


void game_setup(entity_t* entities, ecs_world_t* world_flecs){
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;

    ECS_COMPONENT(world_flecs, entity_t);
    ECS_COMPONENT(world_flecs, sprite_t);
    ECS_COMPONENT(world_flecs, pos2_t);

    InitWindow(screenWidth, screenHeight, "Space Game");
    SetRandomSeed(RANDOM_SEED);

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    int i = 0;

    // Stars
    Texture2D star_texture = LoadTexture("resources/images/star.png");
    for (; i < 21;) {
        entity_t star = {
            .texture = star_texture,
            .src_rect = { .width = star.texture.width, .height = star.texture.height },
            .dest_rect = {
                .width = star.texture.width, .height = star.texture.height,
                .x = GetRandomValue(star.texture.width, WINDOW_WIDTH - star.texture.width),
                .y = GetRandomValue(star.texture.height, WINDOW_HEIGHT - star.texture.height),
            },
            .origin = { 0, 0 },
        };
        entities[i] = star; i++;
    };
    ecs_entity_t Star = ecs_entity(world_flecs, { .name = "star" });
    ecs_set(world_flecs, Star, sprite_t, {
        .texture = &star_texture,
        .src_rect = { .width = star_texture.width, .height = star_texture.height },
    });
    ecs_set(world_flecs, Star, pos2_t, {
        .dest_rect = {
            .width = star_texture.width, .height = star_texture.height,
            .x = GetRandomValue(star_texture.width, WINDOW_WIDTH - star_texture.width),
            .y = GetRandomValue(star_texture.height, WINDOW_HEIGHT - star_texture.height),
        },
        .origin = { 0, 0 },
    });

    // Meteor
    Texture2D meteor_texture = LoadTexture("resources/images/meteor.png");
    entity_t meteor = {
        .texture = meteor_texture,
        .src_rect = { .width = meteor.texture.width, .height = meteor.texture.height },
        .dest_rect = {
            .width = meteor.texture.width, .height = meteor.texture.height,
            .x = WINDOW_WIDTH/2,
            .y = WINDOW_HEIGHT/2,
        },
        .origin = { .x = meteor.texture.width/2, .y = meteor.texture.height/2 },
    };
    entities[i] = meteor; i++;
    ecs_entity_t Meteor = ecs_entity(world_flecs, { .name = "meteor" });
    ecs_set(world_flecs, Meteor, sprite_t, {
        .texture = &meteor_texture,
        .src_rect = { .width = meteor_texture.width, .height = meteor_texture.height },
    });
    ecs_set(world_flecs, Meteor, pos2_t, {
        .dest_rect = {
            .width = meteor_texture.width, .height = meteor_texture.height,
            .x = WINDOW_WIDTH/2,
            .y = WINDOW_HEIGHT/2,
        },
        .origin = { .x = meteor_texture.width/2, .y = meteor_texture.height/2 },
    });

    // Laser
    Texture2D laser_texture = LoadTexture("resources/images/laser.png");
    entity_t laser = {
        .texture = laser_texture,
        .src_rect = { .width = laser.texture.width, .height = laser.texture.height },
        .dest_rect = {
            .width = laser.texture.width, .height = laser.texture.height,
            .x = laser.texture.width + 25,
            .y = WINDOW_HEIGHT - (laser.texture.height + 25),
        },
    };
    entities[i] = laser; i++;
    ecs_entity_t Laser = ecs_entity(world_flecs, { .name = "laser" });
    ecs_set(world_flecs, Laser, sprite_t, {
        .texture = &laser_texture,
        .src_rect = { .width = laser_texture.width, .height = laser_texture.height },
    });
    ecs_set(world_flecs, Laser, pos2_t, {
        .dest_rect = {
            .width = laser_texture.width, .height = laser_texture.height,
            .x = laser_texture.width + 25,
            .y = WINDOW_HEIGHT - (laser_texture.height + 25),
        },
        .origin = { .x = laser_texture.width/2, .y = laser_texture.height/2 },
    });

    // Player
    Texture2D player_texture = LoadTexture("resources/images/player.png");
    entity_t player = {
        .texture = player_texture,
        .src_rect = { .width = player.texture.width, .height = player.texture.height },
        .dest_rect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, player.texture.width, player.texture.height },
        .origin = { player.texture.width/2, player.texture.height/2 },
    };
    entities[i] = player; i++;
    ecs_entity_t Player = ecs_entity(world_flecs, { .name = "player" });
    ecs_set(world_flecs, Player, sprite_t, {
        .texture = &player_texture,
        .src_rect = { .width = player_texture.width, .height = player_texture.height },
    });
    ecs_set(world_flecs, Player, pos2_t, {
        .dest_rect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, player_texture.width, player_texture.height },
        .origin = { player_texture.width/2, player_texture.height/2 },
    });
}

void game_loop(entity_t* entities, ecs_world_t* world_flecs) {
    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        float delta_time = GetFrameTime();
        
        // Update
        //----------------------------------------------------------------------------------

        player_controls(&entities[23], delta_time);

        //----------------------------------------------------------------------------------
        
        game_render(entities);
    }
}

void game_destroy(entity_t* entities, ecs_world_t* world_flecs) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < MAX_NUM_ENTITIES; i++) {
        UnloadTexture(entities[i].texture);
    }

    ecs_fini(world_flecs);
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------
}

void game_render(entity_t* entities) {
    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(DARKBLUE);

        DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
        for (int i = 0; i < MAX_NUM_ENTITIES; i++) {
            DrawTexturePro(entities[i].texture, entities[i].src_rect, entities[i].dest_rect, entities[i].origin, 0.f, WHITE);
        }

    EndDrawing();
    //----------------------------------------------------------------------------------
}

void player_controls(entity_t* entities, float delta_time) {
    Vector2 player_direction = { .x = 0, .y = 0 };
    float player_speed = 300.f;

    player_direction.x = (float) (IsKeyDown(KEY_D) - IsKeyDown(KEY_A));
    player_direction.y = (float) (IsKeyDown(KEY_S) - IsKeyDown(KEY_W));

    player_direction = Vector2Normalize(player_direction);
    Vector2 player_movement = Vector2Scale(player_direction, player_speed);

    entities->dest_rect.x += player_movement.x * delta_time;
    entities->dest_rect.y += player_movement.y * delta_time;
}