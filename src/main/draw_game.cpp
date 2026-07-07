#include "include/game.hpp"

#include "raylib.h"

#include <cmath>



///----------------------------------------------------------------------------
/// Draws a 2D Hexidecimal shape in the center of [x, y] coordinate with given
/// size and color.
///
/// @note You can reed this to understand https://www.redblobgames.com/grids/hexagons/
///----------------------------------------------------------------------------
void drawHex(float x, float y, float size, Color color);


void Game::drawGame() {

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

        // if ((frameCounter/20)%2) DrawText("are you ready?", 160, 500, 50, BLACK);

        DrawRectangleLinesEx((Rectangle){ 0, 0, screen_width, screen_height }, 16, BLACK);

    EndTextureMode();

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, (Rectangle){ 0, 0, (float)target.texture.width, -(float)target.texture.height },
            (Rectangle){ 0, 0, (float)target.texture.width, (float)target.texture.height }, (Vector2){ 0, 0 }, 0.0f, WHITE);

        // TODO: Draw everything that requires to be drawn at this point, maybe UI?

    EndDrawing();
}


void drawHex(float x, float y, float size, Color color)
{
    float width = 2 * size;
    float height = sqrt(3) * size;
    DrawRectangle(x - width / 4, y - height / 2 , width / 2, height, color);
    DrawTriangle( { x + width / 4, y - height / 2 }
                , { x + width / 4, y + height / 2 }
                , { x + width / 2, y }
                , color );
    DrawTriangle( { x - width / 4, y - height / 2 }
                , { x - width / 2, y }
                , { x - width / 4, y + height / 2 }
                , color );
    // DrawCircle(x, y, 5, RED);
}

// void drawHex3D(float x, float y, float z, float size, Color color)
// {
//     float width = 2 * size;
//     float height = size;
//     float length = sqrt(3) * size;
//     Vector3 coordinats { x, y - height / 2, z };
//     size += (width - length) / sqrt(3);

//     DrawCylinder(coordinats, size, size, size, 6, color);
//     DrawCylinderWires(coordinats, size, size, size, 6, BLACK);
// }
