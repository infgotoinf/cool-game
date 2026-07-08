#include "include/game.hpp"

#include "raylib.h"
#include "window_codes.hpp"

#include <cmath>



///----------------------------------------------------------------------------
/// Draws a 2D Hexidecimal shape in the center of [x, y] coordinate with given
/// size and color.
///
/// @note You can reed this to understand https://www.redblobgames.com/grids/hexagons/
///----------------------------------------------------------------------------
void drawHexTable(float x, float y, float size, Color color);


///----------------------------------------------------------------------------
/// Same as Raylibs DrawRectangle(), exept it draw rectangle up.
///----------------------------------------------------------------------------
void drawRectangleUp(float x, float y, float width, float height, Color color)
{
    DrawRectangle(x, y - height, width, height, color);
};


const char* formatTime(float time)
{
    return TextFormat( "%02d:%02d;%02d"
                     , static_cast<int>(time / 60)
                     , static_cast<int>(time) % 60
                     , static_cast<int>(time * 100) % 100);
};


void Game::drawGame() {

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture,
    // it could be useful for scaling or further shader postprocessing
    BeginTextureMode(target);
        // ClearBackground(RAYWHITE);

        // In Krita I properly allign all those objects, but still we need to hardcode
        // a lot of values
        float size_10th = screen_height * 0.1;

        // Interface base
        DrawTexture(textures["game_back"], size_10th, 0, WHITE);
        DrawTexture(textures["game_middle"], size_10th, size_10th, WHITE);
        DrawTexture(textures["game_front"], 0, 0, WHITE);

        // Bar
        DrawRectangle(size_10th * 8.55, size_10th * 1.55, size_10th * 0.85, size_10th * 6.5, DARKGRAY);
        drawRectangleUp(size_10th * 8.55, size_10th * 8.05, size_10th * 0.85, size_10th * 4.5, PURPLE);
        DrawRectangle(size_10th * 8.05, size_10th * 7.9, size_10th * 1.85, size_10th * 1.8, PURPLE);
        DrawTexture(textures["game_bar_face"], size_10th * 8, size_10th * 8, WHITE);
        DrawTexture(textures["game_bar"], size_10th * 8, size_10th * 1.5, WHITE);

        // Hex table
        drawHexTable(size_10th * 2.5 + size_10th / 7, size_10th * 8 - size_10th / 7, size_10th * 2, BLACK);
        DrawTexture(textures["game_hex_table"], size_10th * 0.5, size_10th * 6, WHITE);

        // Time
        DrawText(formatTime(GetTime() - game_start_timestamp), 90, 13, 40, DARKGRAY);

        // Curse Value
        DrawText( TextFormat("%10.2fX", curse_value)
                // idk if this hardcoded value will work, but ok
                , size_10th * 9 - MeasureText(TextFormat("%10.2fX", curse_value), 40) * 0.75
                , size_10th, 40, PURPLE);

        // Pause
        // DrawRectangle(18, 11, 40, 40, WHITE);
        DrawTexture(textures["game_pause_icon"], 20, 13, BLACK);
        if ( IsMouseButtonPressed(MOUSE_BUTTON_LEFT)
          && CheckCollisionPointRec(GetMousePosition(), {18, 11, 40, 40}))
        {
            current_window = WindowID::PAUSE;
        }

        // Draw


    EndTextureMode();

    // Render to screen (main framebuffer)
    BeginDrawing();
        ClearBackground(RAYWHITE);

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height },
            { 0, 0, (float)target.texture.width, (float)target.texture.height }, { 0, 0 }, 0.0f, WHITE);

        // DrawTexture(textures["game_front"], 0, 0, BLANK);
        // TODO: Draw everything that requires to be drawn at this point, maybe UI?

    EndDrawing();
}


void drawHexTable(float x, float y, float size, Color color)
{
    float width = 2 * size;
    float height = sqrt(3) * size;

    // Top-left
    DrawTriangle( { x - width / 4, y - height / 2 }
                , { x - width / 2, y }
                , { x, y }
                , YELLOW );
    // Top
    DrawTriangle( { x - width / 4, y - height / 2 }
                , { x, y }
                , { x + width / 4, y - height / 2 }
                , GREEN );
    // Top-right
    DrawTriangle( { x + width / 4, y - height / 2 }
                , { x, y }
                , { x + width / 2, y }
                , RED );
    // Down-right
    DrawTriangle( { x, y }
                , { x + width / 4, y + height / 2 }
                , { x + width / 2, y }
                , PINK );
    // Down
    DrawTriangle( {x, y}
                , { x - width / 4, y + height / 2 }
                , { x + width / 4, y + height / 2 }
                , ORANGE );
    // Down-left
    DrawTriangle( { x, y }
                , { x - width / 2, y }
                , { x - width / 4, y + height / 2 }
                , SKYBLUE );

    // DrawCircle(x, y, 5, RED);
}

// void drawHexTable3D(float x, float y, float z, float size, Color color)
// {
//     float width = 2 * size;
//     float height = size;
//     float length = sqrt(3) * size;
//     Vector3 coordinats { x, y - height / 2, z };
//     size += (width - length) / sqrt(3);

//     DrawCylinder(coordinats, size, size, size, 6, color);
//     DrawCylinderWires(coordinats, size, size, size, 6, BLACK);
// }
