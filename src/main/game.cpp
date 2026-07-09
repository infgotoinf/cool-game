#include "include/game.hpp"

#include "raylib.h"



void loadAnimationFrames(const char *dir_path, std::vector<Texture2D> *frames)
{
    FilePathList frame_files = LoadDirectoryFiles(dir_path);
    for (int i = 0; i < frame_files.count; ++i)
    {
        Image image = LoadImage(frame_files.paths[i]);
        ImageColorInvert(&image);
        ImageColorTint(&image, {100, 100, 100, 255});
        frames->push_back(LoadTextureFromImage(image));
        UnloadImage(image);
    }
    UnloadDirectoryFiles(frame_files);
}


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

    loadAnimationFrames("resources/game/guy/walk", &animations["game_guy_walk"]);
    loadAnimationFrames("resources/game/guy/idle", &animations["game_guy_idle"]);
    for (Texture2D frame : animations["game_guy_walk"])
    {
        Image image = LoadImageFromTexture(frame);
        ImageFlipHorizontal(&image);
        animations["game_guy_walk_back"].push_back(LoadTextureFromImage(image));
        UnloadImage(image);
    }
    for (Texture2D frame : animations["game_guy_idle"])
    {
        Image image = LoadImageFromTexture(frame);
        ImageFlipHorizontal(&image);
        animations["game_guy_idle_back"].push_back(LoadTextureFromImage(image));
        UnloadImage(image);
    }

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

    for ( std::map<const char*, std::vector<Texture>>::iterator it = animations.begin()
        ; it != animations.end(); ++it)
    {
        for (Texture2D &texture : it->second)
        {
            UnloadTexture(texture);
        }
    }

    CloseWindow();        // Close window and OpenGL context
    //-------------------------------------------------------------------------
}


void Game::resetGame()
{
    camera = { 0 };
    camera.target = { 0, 0 };
    camera.offset = { 0, 0 };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;

    is_gameover = false;
    is_tutorial_over = false;
    curse_value = SECONDS_TO_CURSE;
    curse_drain_speed = 1.0f;

    SetRandomSeed(GetTime());
    guys.clear();
    for (int i = 0; i < 10; ++i)
        guys.push_back(createRandomGuy());

    sortGuysByPosY();

    game_start_timestamp = GetTime();
}


void Game::shakeScreen(int shake_ammount, float shake_stopper)
{
    camera.offset = { 0, 0 };
    camera.rotation = 0.0f;

    camera.offset.y += GetRandomValue(-shake_ammount, shake_ammount);
    camera.offset.x += GetRandomValue(-shake_ammount, shake_ammount);
}
