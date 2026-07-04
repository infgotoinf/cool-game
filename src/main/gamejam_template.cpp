/*******************************************************************************************
*
*   raylib gamejam template
*
*   Code licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2022-2026 Ramon Santamaria (@raysan5)
*
********************************************************************************************/
#include "include/gamejam_template.hpp"

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
// Update and draw frame
void drawGamejamTemplate(RenderTexture2D target)
{
    static const int screenWidth = 720;
    static const int screenHeight = 720;

    static int frameCounter = 0;

    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point

    frameCounter++;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture,
    // it could be useful for scaling or further shader postprocessing
    BeginTextureMode(target);
        ClearBackground(RAYWHITE);

        // TODO: Draw your game screen here

        DrawRectangle(70, 90, 200, 200, BLACK);
        DrawRectangle(70 + 16, 90 + 16, 200 - 32, 200 - 32, RAYWHITE);
        DrawText("raylib", 70 + 200 - MeasureText("raylib", 40) - 32, 90 + 200 - 40 - 24, 40, BLACK);

        DrawText("6.x", 290, 90 - 26, 280, BLACK);
        DrawText("GAMEJAM", 70, 90 + 210, 120, MAROON);

        if ((frameCounter/20)%2) DrawText("are you ready?", 160, 500, 50, BLACK);

        DrawRectangleLinesEx((Rectangle){ 0, 0, screenWidth, screenHeight }, 16, BLACK);

    EndTextureMode();

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        // TODO: Draw everything that requires to be drawn at this point, maybe UI?

    EndDrawing();
    //----------------------------------------------------------------------------------
}
