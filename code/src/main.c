#include "settings.h"

typedef struct Entity {
    Texture2D texture;
    Rectangle src_rect;
    Rectangle dest_rect;
    Vector2 origin;
} entity_t;



void setup();
void game_loop(entity_t* entities);
void destroy_game();

int main(int argc, char const *argv[]) {

    setup();

    entity_t entities[MAX_NUM_ENTITIES];
    

    entity_t player = {
        .texture = LoadTexture("resources/images/player.png"),
        .src_rect = { 0, 0, player.texture.width, player.texture.height },
        .dest_rect = { 100, 150, player.texture.width, player.texture.height },
        .origin = { 0, 0 },
    };
    entities[0] = player;

    Texture2D star_texture = LoadTexture("resources/images/star.png");
    for (int i = 1; i < MAX_NUM_ENTITIES; i++) {
        entity_t star = {
            .texture = star_texture,
            .src_rect = { .width = star.texture.width, .height = star.texture.height },
            .dest_rect = {
                .width = star.texture.width, .height = star.texture.height,
                .x = GetRandomValue(star.texture.width, WINDOW_WIDTH - star.texture.width),
                .y = GetRandomValue(star.texture.height, WINDOW_HEIGHT - star.texture.height)
            }
        };

        entities[i] = star;
    }


    game_loop(entities);
    destroy_game();

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
    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here

        entities[0].dest_rect.x += 0.1;
        //----------------------------------------------------------------------------------

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

void destroy_game() {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

}