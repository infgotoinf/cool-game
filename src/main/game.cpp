#include "include/game.hpp"

#include "raylib.h"



enum AnimationType : unsigned char
{
    GUY_ANIMATION
   ,NOT_GUY_ANIMATION
};

void loadAnimationFrames(const char *dir_path, std::vector<Texture2D> *frames, AnimationType animation_type)
{
    FilePathList frame_files = LoadDirectoryFiles(dir_path);
    for (int i = 0; i < frame_files.count; ++i)
    {
        Image image = LoadImage(frame_files.paths[i]);
        if (animation_type == GUY_ANIMATION)
        {
            ImageColorInvert(&image);
            ImageColorTint(&image, {100, 100, 100, 255});
        }
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
    InitAudioDevice();

    // Load buttons
    textures["buttons_begin"]    = LoadTexture("resources/buttons/begin.png");
    textures["buttons_continue"] = LoadTexture("resources/buttons/continue.png");
    textures["buttons_end"]      = LoadTexture("resources/buttons/end.png");
    textures["buttons_restart"]  = LoadTexture("resources/buttons/restart.png");

    // Load static textures
    textures["game_back"]         = LoadTexture("resources/game/back.png");
    textures["game_middle"]       = LoadTexture("resources/game/middle.png");
    textures["game_almost_front"] = LoadTexture("resources/game/almost_front.png");
    textures["game_front"]        = LoadTexture("resources/game/front.png");
    textures["game_hex_table"]    = LoadTexture("resources/game/hex_table.png");
    textures["game_curtains"]     = LoadTexture("resources/game/curtains.png");
    textures["game_bar"]          = LoadTexture("resources/game/bar.png");
    textures["game_bar_face"]     = LoadTexture("resources/game/bar_face.png");
    textures["game_pause_icon"]   = LoadTexture("resources/game/pause_icon.png");

    textures["menu_back"]   = LoadTexture("resources/menu/back.png");
    textures["menu_front"]  = LoadTexture("resources/menu/front.png");
    textures["menu_papers"] = LoadTexture("resources/menu/papers.png");
    textures["menu_text"]   = LoadTexture("resources/menu/text.png");

    // Load animations
    loadAnimationFrames("resources/game/guy/walk", &animations["game_guy_walk"], GUY_ANIMATION);
    loadAnimationFrames("resources/game/guy/idle", &animations["game_guy_idle"], GUY_ANIMATION);
    // Create "back" variations
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

    // Load cursors
    loadAnimationFrames("resources/game/cursor/free", &animations["game_cursor_free"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/cursor/grab", &animations["game_cursor_grab"], NOT_GUY_ANIMATION);

    // Load fonts
    font = LoadFontEx("resources/fonts/my-old-remington/myoldrem.ttf", 64, 0, 0);
    // font = LoadFontEx("resources/fonts/x-typewriter/XTypewriter-Regular.ttf", 64, 0, 0);
    // font = LoadFontEx("resources/fonts/x-typewriter/XTypewriter-Bold.ttf", 64, 0, 0);

    // Load sounds
    sounds["button_click"] = LoadSound("resources/audio/button/click.wav");
    sounds["button_hover"] = LoadSound("resources/audio/button/hover.wav");

    // Load music
    music["inadequate"] = LoadMusicStream("resources/music/inadequate.mp3");
    for ( std::map<const char*, Music>::iterator it = music.begin()
        ; it != music.end(); ++it)
    {
    }

    // Gen noise
    for (int i = 0; i < 16; ++i)
    {
        Image noise_img;
        noise_img = GenImageWhiteNoise(screen_width, screen_height, 0.5f);
        noise.push_back(LoadTextureFromImage(noise_img));
        UnloadImage(noise_img);
    }

    Image vignette_img = GenImageGradientRadial(screen_width, screen_height, 0.0f, {0, 0, 0, 0}, BLACK);
    vignette = LoadTextureFromImage(vignette_img);
    UnloadImage(vignette_img);

    // Target setup
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
    for ( std::map<const char*, Sound>::iterator it = sounds.begin()
        ; it != sounds.end(); ++it)
    {
        UnloadSound(it->second);
    }
    for (Texture2D &texture : noise)
    {
        UnloadTexture(texture);
    }
    UnloadTexture(vignette);

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

    SetRandomSeed(GetTime() * 100);
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
