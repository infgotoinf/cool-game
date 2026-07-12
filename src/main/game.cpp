#include "include/game.hpp"

#include "raylib.h"

#include <cmath>
#include <vector>



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

    // Objects
    textures["game_object_book_blindness"] = LoadTexture("resources/game/objects/books/blindness.png");
    textures["game_object_knife"]  = LoadTexture("resources/game/objects/knife.png");
    textures["game_object_spider"] = LoadTexture("resources/game/objects/dead/spider.png");
    loadAnimationFrames("resources/game/objects/spider", &animations["game_object_spider"], NOT_GUY_ANIMATION);
    textures["game_object_bird"] = LoadTexture("resources/game/objects/dead/bird.png");
    loadAnimationFrames("resources/game/objects/bird/fly", &animations["game_object_bird_fly"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/objects/bird/idle", &animations["game_object_bird_idle"], NOT_GUY_ANIMATION);

    // Image image = LoadImageFromTexture(textures["game_object_bird"]);
    // ImageFlipHorizontal(&image);
    // animations["game_object_bird_back"].push_back(LoadTextureFromImage(image));
    // UnloadImage(image);
    for (Texture2D frame : animations["game_object_bird_fly"])
    {
        Image image = LoadImageFromTexture(frame);
        ImageFlipHorizontal(&image);
        animations["game_object_bird_fly_back"].push_back(LoadTextureFromImage(image));
        UnloadImage(image);
    }
    for (Texture2D frame : animations["game_object_bird_idle"])
    {
        Image image = LoadImageFromTexture(frame);
        ImageFlipHorizontal(&image);
        animations["game_object_bird_idle_back"].push_back(LoadTextureFromImage(image));
        UnloadImage(image);
    }

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

    loadAnimationFrames("resources/game/face/calm", &animations["game_face_calm"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/face/freakout", &animations["game_face_freakout"], NOT_GUY_ANIMATION);

    // Load cursors
    loadAnimationFrames("resources/game/cursor/free", &animations["game_cursor_free"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/cursor/grab", &animations["game_cursor_grab"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/cursor/ready", &animations["game_cursor_ready"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/cursor/knife/hold", &animations["game_cursor_knife_hold"], NOT_GUY_ANIMATION);
    loadAnimationFrames("resources/game/cursor/knife/swing", &animations["game_cursor_knife_swing"], NOT_GUY_ANIMATION);

    // Load fonts
    font = LoadFontEx("resources/fonts/my-old-remington/myoldrem.ttf", 64, 0, 0);

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

    // Triangles
    float trinagle_x = 72 * 2.5 + 2;
    float trinagle_y = 72 * 8 - 5;
    float trinagle_size = 72 * 2 + 6;

    float trinagle_width = 2 * trinagle_size;
    float trinagle_height = sqrt(3) * trinagle_size;

    hex_table_trinagles[0] = {  trinagle_x - trinagle_width / 4, trinagle_y - trinagle_height / 2
                ,  trinagle_x - trinagle_width / 2, trinagle_y
                ,  trinagle_x, trinagle_y
                , WHITE
                , BLACK }; // Top-left

    hex_table_trinagles[1] = { { trinagle_x - trinagle_width / 4, trinagle_y - trinagle_height / 2 }
                , { trinagle_x, trinagle_y }
                , { trinagle_x + trinagle_width / 4, trinagle_y - trinagle_height / 2 }
                , WHITE
                , BLACK }; // Top

    hex_table_trinagles[2] = { { trinagle_x + trinagle_width / 4, trinagle_y - trinagle_height / 2 }
                , { trinagle_x, trinagle_y }
                , { trinagle_x + trinagle_width / 2, trinagle_y }
                , WHITE
                , BLACK }; // Top-right

    hex_table_trinagles[3] = { { trinagle_x, trinagle_y }
                , { trinagle_x + trinagle_width / 4, trinagle_y + trinagle_height / 2 }
                , { trinagle_x + trinagle_width / 2, trinagle_y }
                , WHITE
                , BLACK }; // Down-right

    hex_table_trinagles[4] = { {trinagle_x, trinagle_y}
                , { trinagle_x - trinagle_width / 4, trinagle_y + trinagle_height / 2 }
                , { trinagle_x + trinagle_width / 4, trinagle_y + trinagle_height / 2 }
                , WHITE
                , BLACK }; // Down

    hex_table_trinagles[5] = { { trinagle_x, trinagle_y }
                , { trinagle_x - trinagle_width / 2, trinagle_y }
                , { trinagle_x - trinagle_width / 4, trinagle_y + trinagle_height / 2 }
                , WHITE
                , BLACK }; // Down-left

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

    for (DragableObject *&obj : dragable_objects)
    {
        delete obj;
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
    is_holding_a_knife = false;
    curse_value = SECONDS_TO_CURSE;
    curse_drain_speed = 1.0f;

    SetRandomSeed(GetTime() * 100);
    guys.clear();
    for (int i = 0; i < 10; ++i)
        guys.push_back(createRandomGuy());

    sortGuysByPosY();

    dragable_objects.clear();
    createDragableObject(std::vector<Texture2D>{}, textures["game_object_knife"], { 400, 500 }, 30, KNIFE, NO);

    game_start_timestamp = GetTime();

    for (int i = 0; i < 6; ++i)
    {
        hex_table_trinagles[i].is_active = false;
    }
}


Color Triangle::GetColor()
{
    if (is_active)
    {
        return color;
    }
    return inactive_color;
}
