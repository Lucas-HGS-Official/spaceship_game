#include "raylib.h"
#include "settings.h"


void setup();
void game_loop(Texture2D* texture_to_draw, Rectangle* texture_src_rect, Rectangle* texture_dest_rect);
void destroy_game();

int main(int argc, char const *argv[]) {

    setup();
    
    Texture2D player_texture = LoadTexture("resources/images/player.png");

    Rectangle player_src_rect = { 0, 0, player_texture.width, player_texture.height };
    Rectangle player_dest_rect = { 0, 0, player_texture.width, player_texture.height };
    Vector2 player_origin = { 0, 0 };


    //     // Main game loop
    // while (!WindowShouldClose()) {  // Detect window close button or ESC key
    //     // Update
    //     //----------------------------------------------------------------------------------
    //     // TODO: Update your variables here
    //     //----------------------------------------------------------------------------------

    //     // Draw
    //     //----------------------------------------------------------------------------------
    //     BeginDrawing();

    //         // ClearBackground(RAYWHITE);

    //         DrawText("Congrats! You created your first window!", 190, 200, 20, LIGHTGRAY);
    //         DrawRectangle(WINDOW_WIDTH/2 - 32/2, WINDOW_HEIGHT/2 - 32/2,32,32, WHITE);
    //         DrawTexture(player_texture, WINDOW_WIDTH/2 - player_texture.width/2, WINDOW_HEIGHT/2 - player_texture.height/2, WHITE);

    //     EndDrawing();
    //     //----------------------------------------------------------------------------------
    // }
    game_loop(&player_texture, &player_src_rect, &player_dest_rect);
    destroy_game();

    return 0;
}


void setup(){
    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = WINDOW_WIDTH;
    const int screenHeight = WINDOW_HEIGHT;

    InitWindow(screenWidth, screenHeight, "Space Game");

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

}

void game_loop(Texture2D* texture_to_draw, Rectangle* texture_src_rect, Rectangle* texture_dest_rect) {
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
            DrawTexturePro(*texture_to_draw, *texture_src_rect, *texture_dest_rect, (Vector2) { 0.f, 0.f }, 0.f, WHITE);

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