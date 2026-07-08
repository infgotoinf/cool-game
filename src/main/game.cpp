#include "include/game.hpp"

#include "raylib.h"



Game::Game()
{
    // Initialization
    //-------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "HexAgony");

    textures["game_back"]       = LoadTexture("resources/game/back.png");
    textures["game_middle"]     = LoadTexture("resources/game/middle.png");
    textures["game_front"]      = LoadTexture("resources/game/front.png");
    textures["game_bar"]        = LoadTexture("resources/game/bar.png");
    textures["game_bar_face"]   = LoadTexture("resources/game/bar_face.png");
    textures["game_hex_table"]  = LoadTexture("resources/game/hex_table.png");
    textures["game_pause_icon"] = LoadTexture("resources/game/pause_icon.png");

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

    // https://stackoverflow.com/questions/2850312/use-of-for-each-on-map-elements
    for ( std::map<const char*, Texture2D>::iterator it = textures.begin()
        ; it != textures.end(); ++it)
    {
        UnloadTexture(it->second);
    }

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
