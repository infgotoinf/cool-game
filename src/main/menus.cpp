#include "include/game.hpp"

#include "raylib.h"



///----------------------------------------------------------------------------
/// Centers rectangle with width and height at x and y coorginats and returns
/// it as Raylib Rectangle.
///
/// @note By default Raylib creates a Rectange from x and y coordinats with
/// width and height. It creates it from the corner, that's why we need this.
///----------------------------------------------------------------------------
Rectangle centerRect(int x, int y, float width, float height);


bool Game::customButton(Texture2D& texture, int pos_x, int pos_y, ButtonStatus* past_status)
{
    if (CheckCollisionPointRec(GetMousePosition(), centerRect( pos_x, pos_y
                                                             , texture.width, texture.height)))
    {
        DrawTexture(texture, pos_x - texture.width / 2, pos_y - texture.height / 2, { 255, 255, 255, 255});
        if (*past_status == NOT_HOVERED) PlaySound(sounds["button_hover"]);
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            PlaySound(sounds["button_click"]);
            return true;
        }
        *past_status = HOVERED;
    }
    else
    {
        DrawTexture(texture, pos_x - texture.width / 2, pos_y - texture.height / 2, { 255, 255, 255, 200});
        *past_status = NOT_HOVERED;
    }
    return false;
}


void Game::drawMainMenu()
{
    BeginDrawing();
        // ClearBackground(DARKGRAY);
        // GuiSetStyle(DEFAULT, TEXT_SIZE, 20);

        // if (GuiButton( centerRect( screen_width / 2
        //                          , screen_height / 2 + 45
        //                          , 250, 40)
        //              , "Begin"))
        // {
        //     HideCursor();
        //     current_window = WindowID::GAME;
        // }

        // if (GuiButton( centerRect( screen_width / 2
        //                          , screen_height / 2 + 90
        //                          , 250, 40)
        //              , "End"))
        // {}
            // current_window = WindowID::GAME_3D_EXAMPLE;

        DrawTexture(textures["menu_back"],   -36, -36, WHITE);
        DrawTexture(textures["menu_front"],  -36 + getCursorPosFromCenter().x / 50, -36 + getCursorPosFromCenter().y / 50, WHITE);
        DrawTexture(textures["menu_text"],   -36 + getCursorPosFromCenter().x / 30, -36 + getCursorPosFromCenter().y / 30, WHITE);
        DrawTexture(textures["menu_papers"], -36 + getCursorPosFromCenter().x / 10, -36 + getCursorPosFromCenter().y / 10, WHITE);

        static ButtonStatus menu_begin_status = NOT_HOVERED;
        static ButtonStatus menu_end_status = NOT_HOVERED;
        if (customButton(textures["buttons_begin"], screen_width * 0.5 + getCursorPosFromCenter().x / 20
                                                  , screen_height * 0.5 + 50 + getCursorPosFromCenter().y / 20, &menu_begin_status))
        {
            HideCursor();
            current_window = WindowID::GAME;
        }
        else if (customButton(textures["buttons_end"], screen_width * 0.5 + getCursorPosFromCenter().x / 20
                                                  , screen_height * 0.5 + 100 + getCursorPosFromCenter().y / 20, &menu_end_status))
        {
        }

    EndDrawing();
}


void Game::drawPause()
{
    ShowCursor();
    BeginDrawing();
        // ClearBackground(RAYWHITE);

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height },
            { 0, 0, (float)target.texture.width, (float)target.texture.height }, { 0, 0 }, 0.0f, WHITE);

        DrawRectangle(0, 0, screen_width, screen_height, { 0, 0, 0, 200 });

        // if (GuiButton( centerRect( screen_width / 2
        //                          , screen_height / 2 - 45
        //                          , 250, 40)
        //              , "Continue"))
        // {
        //     HideCursor();
        //     current_window = WindowID::GAME;
        //     game_start_timestamp += GetTime() - game_pause_timestamp;
        // }

        // if (GuiButton( centerRect( screen_width / 2
        //                          , screen_height / 2
        //                          , 200, 40)
        //              , "Restart"))
        // {
        //     HideCursor();
        //     current_window = WindowID::GAME;
        //     resetGame();
        // }

        // if (GuiButton( centerRect( screen_width / 2
        //                          , screen_height / 2 + 45
        //                          , 150, 40)
        //              , "End"))
        //     current_window = WindowID::MAIN_MENU;

        static ButtonStatus pause_continue_status = NOT_HOVERED;
        static ButtonStatus pause_restart_status = NOT_HOVERED;
        static ButtonStatus pause_end_status = NOT_HOVERED;
        if (customButton(textures["buttons_continue"], screen_width / 2, screen_height / 2 - 50, &pause_continue_status))
        {
            HideCursor();
            current_window = WindowID::GAME;
            game_start_timestamp += GetTime() - game_pause_timestamp;
        }
        else if (customButton(textures["buttons_restart"], screen_width / 2, screen_height / 2, &pause_restart_status))
        {
            HideCursor();
            current_window = WindowID::GAME;
            resetGame();
        }
        else if (customButton(textures["buttons_end"], screen_width / 2, screen_height / 2 + 50, &pause_end_status))
        {
            current_window = WindowID::MAIN_MENU;
        }

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
