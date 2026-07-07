#include "include/game.hpp"

#include "raylib.h"



Game::Game()
{
    // Initialization
    //-------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "HexAgony");

    // Image python_img = LoadImage("resources/python-guy.png");
    // ImageResize(&python_img, image_size, image_size);
    // python_tex = LoadTextureFromImage(python_img);
    // UnloadImage(python_img);

    // Image cpp_img = LoadImage("resources/cpp-guy.png");
    // ImageResize(&cpp_img, image_size, image_size);
    // cpp_tex = LoadTextureFromImage(cpp_img);
    // UnloadImage(cpp_img);

    target = LoadRenderTexture(screen_width, screen_height);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

    SetTargetFPS(60);

    resetGame();
}


Game::~Game()
{
    // De-Initialization
    //-------------------------------------------------------------------------
    UnloadRenderTexture(target);

    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //-------------------------------------------------------------------------
}


void Game::resetGame()
{
    camera = { 0 };
    camera.target = (Vector2){ 0, 0 };
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    is_gameover = false;

    game_start_timestamp = GetTime();
}


void Game::shakeScreen(int shake_ammount, float shake_stopper)
{
    camera.offset = (Vector2){ 0, 0 };
    camera.rotation = 0.0f;

    camera.offset.y += GetRandomValue(-shake_ammount, shake_ammount);
    camera.offset.x += GetRandomValue(-shake_ammount, shake_ammount);
}


// double Game::showTime(double cur_time)
// {
//     double ret_time = cur_time - time;
//     DrawText(TextFormat("%06.3f", ret_time)
//            , 30, 30, 20, WHITE);
//     return ret_time;
// }
