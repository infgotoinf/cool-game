#include "include/game.hpp"

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"



///----------------------------------------------------------------------------
/// Centers rectangle with width and height at x and y coorginats and returns
/// it as Raylib Rectangle.
///
/// @note By default Raylib creates a Rectange from x and y coordinats with
/// width and height. It creates it from the corner, that's why we need this.
///----------------------------------------------------------------------------
Rectangle centerRect(int x, int y, float width, float height);


void Game::drawMainMenu()
{
    BeginDrawing();
        ClearBackground(DARKGRAY);
        GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

        if (GuiButton( centerRect( screen_width / 2
                                 , screen_height / 2
                                 , 250, 40)
                     , "Begin"))
            current_window = WindowID::GAME;

        if (GuiButton( centerRect( screen_width / 2
                                 , screen_height / 2 + 45
                                 , 250, 40)
                     , "End"))
        {}
            // current_window = WindowID::GAME_3D_EXAMPLE;

    EndDrawing();
}


void Game::drawPause()
{
    BeginDrawing();
        // ClearBackground(RAYWHITE);

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height },
            { 0, 0, (float)target.texture.width, (float)target.texture.height }, { 0, 0 }, 0.0f, WHITE);

        DrawRectangle(0, 0, screen_width, screen_height, { 0, 0, 0, 200 });

        if (GuiButton( centerRect( screen_width / 2
                                 , screen_height / 2 - 45
                                 , 250, 40)
                     , "Continue"))
        {
            game_start_timestamp += GetTime() - game_pause_timestamp;
            current_window = WindowID::GAME;
        }

        if (GuiButton( centerRect( screen_width / 2
                                 , screen_height / 2
                                 , 200, 40)
                     , "Restart"))
        {
            current_window = WindowID::GAME;
            resetGame();
        }

        if (GuiButton( centerRect( screen_width / 2
                                 , screen_height / 2 + 45
                                 , 150, 40)
                     , "End"))
            current_window = WindowID::MAIN_MENU;

    EndDrawing();
}


Rectangle centerRect(int x, int y, float width, float height)
{
    // Don't want to static_cast<float>(), so cast int to float this way
    return Rectangle{ width / -2 + x
                    , height / -2 + y
                    , width
                    , height };
}
