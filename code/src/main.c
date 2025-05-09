#include "settings.h"


void setup(ecs_world_t* ecs_world);
void game_loop(void);
void destroy_game(ecs_world_t* ecs_world);

int main(int argc, char const *argv[]) {
    ecs_world_t* ecs_world;
    setup(ecs_world);
    game_loop();
    destroy_game(ecs_world);

    return 0;
}


void setup(ecs_world_t* ecs_world){
    // Initialization
    //--------------------------------------------------------------------------------------
    ecs_world = ecs_init();
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;

    InitWindow(screenWidth, screenHeight, "Pokeclone");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

}

void game_loop(void) {
    // Main game loop
    while (!WindowShouldClose()) {  // Detect window close button or ESC key
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

            ClearBackground(RAYWHITE);

            DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

}

void destroy_game(ecs_world_t* ecs_world) {
    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    ecs_fini(ecs_world);
    //--------------------------------------------------------------------------------------

}