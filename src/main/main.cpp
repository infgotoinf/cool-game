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

#include "raylib.h"
#define RAYGUI_IMPLEMENTATION
#include "raygui.h"

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

#include "include/window_codes.hpp"
#include "include/gamejam_template.hpp"
#include "include/python_vc_cpp.hpp"
#include "include/game_3d_example.hpp"
#include "include/paralax_bg_example.hpp"
#include "include/music_example.hpp"



//----------------------------------------------------------------------------------
// Defines and Macros
//----------------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif



//----------------------------------------------------------------------------------
// Global Variables Definition (local to this module)
//----------------------------------------------------------------------------------
static const int screen_width = 720;
static const int screen_height = 720;

static RenderTexture2D target = { 0 };  // Render texture to render our game
static WindowID current_window = WindowID::MAIN_MENU;

// TODO: Define global variables here, recommended to make them static



//----------------------------------------------------------------------------------
// Module Functions Declaration
//----------------------------------------------------------------------------------
static void UpdateDrawFrame(void);      // Update and Draw one frame

Rectangle centerButton(int x, int y, int width, int height);



//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

    // Initialization
    //--------------------------------------------------------------------------------------
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI | FLAG_MSAA_4X_HINT);
    InitWindow(screen_width, screen_height, "raylib gamejam template");

    // TODO: Load resources / Initialize variables at this point

    // Render texture to draw, enables screen_ scaling
    // NOTE: If screen_ is scaled, mouse input should be scaled proportionally
    target = LoadRenderTexture(screen_width, screen_height);
    SetTextureFilter(target.texture, TEXTURE_FILTER_BILINEAR);

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(UpdateDrawFrame, 60, 1);
#else
    SetTargetFPS(60);     // Set our game frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button
    {
        UpdateDrawFrame();
    }
#endif

    // De-Initialization
    //--------------------------------------------------------------------------------------
    UnloadRenderTexture(target);

    // TODO: Unload all loaded resources at this point

    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}



//--------------------------------------------------------------------------------------------
// Module Functions Definition
//--------------------------------------------------------------------------------------------
// Update and draw frame
void UpdateDrawFrame(void)
{
    // Update
    //----------------------------------------------------------------------------------
    // TODO: Update variables / Implement example logic at this point

    if (IsKeyPressed(KEY_Q)) {
        EnableCursor();
        current_window = WindowID::MAIN_MENU;
    }

    switch (current_window) {
    case WindowID::MUSIC_EXAMPLE:
        drawMusicExample();
        break;

    case WindowID::PYTHON_VC_CPP:
        drawPythonVcCpp();
        break;

    case WindowID::GAMEJAM_TEMPLATE:
        drawGamejamTemplate(target);
        break;

    case WindowID::GAME_3D_EXAMPLE:
        drawGame3DExample();
        break;

    case WindowID::PARALAX_BG_EXAMPLE:
        drawParalaxBgExample();
        break;

    case WindowID::MAIN_MENU:
    default:
        BeginDrawing();
            ClearBackground(RAYWHITE);
            GuiSetStyle(DEFAULT, TEXT_SIZE, 20);
            if (GuiButton( centerButton(screen_width / 2, screen_height / 2 - 90, 250, 40)
                         , "Music Example"))
                current_window = WindowID::MUSIC_EXAMPLE;

            if (GuiButton( centerButton(screen_width / 2, screen_height / 2 - 45, 250, 40)
                         , "Python VC C++"))
                current_window = WindowID::PYTHON_VC_CPP;

            if (GuiButton( centerButton(screen_width / 2, screen_height / 2, 250, 40)
                         , "Gamejam Template"))
                current_window = WindowID::GAMEJAM_TEMPLATE;

            if (GuiButton( centerButton(screen_width / 2, screen_height / 2 + 45, 250, 40)
                         , "3D Game"))
                current_window = WindowID::GAME_3D_EXAMPLE;

            if (GuiButton( centerButton(screen_width / 2, screen_height / 2 + 90, 250, 40)
                         , "Paralax BG"))
                current_window = WindowID::PARALAX_BG_EXAMPLE;

        EndDrawing();
    }
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen_ to a texture,
    // it could be useful for scaling or further shader postprocessing

    // Render to screen_ (main framebuffer)
    BeginDrawing();
        DrawText("Press (Q) to exit the example"
               , 20, screen_height - 40, 20, PINK);
    EndDrawing();
    //----------------------------------------------------------------------------------
}


Rectangle centerButton(int x, int y, int width, int height) {
    return Rectangle{ static_cast<float>(width) / -2 + x
                    , static_cast<float>(height) / -1 + y
                    , static_cast<float>(width)
                    , static_cast<float>(height) };
}
