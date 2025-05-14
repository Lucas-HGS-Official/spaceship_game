#include "settings.h"

typedef struct Entity {
    Texture2D texture;
    Rectangle src_rect;
    Rectangle dest_rect;
    Vector2 origin;
} entity_t;



void setup();
void game_loop(entity_t* entities);
void destroy_game(entity_t* entities);

int main(int argc, char const *argv[]) {

    setup();

    entity_t entities[MAX_NUM_ENTITIES];
    
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

    // Meteor
    entity_t meteor = {
        .texture = LoadTexture("resources/images/meteor.png"),
        .src_rect = { .width = meteor.texture.width, .height = meteor.texture.height },
        .dest_rect = {
            .width = meteor.texture.width, .height = meteor.texture.height,
            .x = WINDOW_WIDTH/2,
            .y = WINDOW_HEIGHT/2,
        },
        .origin = { .x = meteor.texture.width/2, .y = meteor.texture.height },
    };
    entities[i] = meteor; i++;

    // Laser
    entity_t laser = {
        .texture = LoadTexture("resources/images/laser.png"),
        .src_rect = { .width = laser.texture.width, .height = laser.texture.height },
        .dest_rect = {
            .width = laser.texture.width, .height = laser.texture.height,
            .x = laser.texture.width + 25,
            .y = WINDOW_HEIGHT - (laser.texture.height + 25),
        },
    };
    entities[i] = laser; i++;

    // Player
    entity_t player = {
        .texture = LoadTexture("resources/images/player.png"),
        .src_rect = { 0, 0, player.texture.width, player.texture.height },
        .dest_rect = { WINDOW_WIDTH/2, WINDOW_HEIGHT/2, player.texture.width, player.texture.height },
        .origin = { player.texture.width/2, player.texture.height/2 },
    };
    entities[i] = player; i++;


    game_loop(entities);
    destroy_game(entities);

    return 0;
}


void setup(){
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;

    InitWindow(screenWidth, screenHeight, "Space Game");
    SetRandomSeed(RANDOM_SEED);

    SetTargetFPS(FPS);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

}

void game_loop(entity_t* entities) {
    Vector2 player_direction = { .x = 2, .y = -1 };
    player_direction = Vector2Normalize(player_direction);
    float player_speed = 100.f;
    Vector2 player_movement = Vector2Scale(player_direction, player_speed);

    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------
        float delta_time = GetFrameTime();

        entities[23].dest_rect.x += player_movement.x * delta_time;
        entities[23].dest_rect.y += player_movement.y * delta_time;

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

}

void destroy_game(entity_t* entities) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    for (int i = 0; i < MAX_NUM_ENTITIES; i++) {
        UnloadTexture(entities[i].texture);
    }

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

}