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

#if defined(PLATFORM_WEB)
    #include <emscripten/emscripten.h>      // Emscripten library
#endif

#include "include/game.hpp"

// #define _DEBUG


//-----------------------------------------------------------------------------
// Defines and Macros
//-----------------------------------------------------------------------------
// Simple log system to avoid printf() calls if required
// NOTE: Avoiding those calls, also avoids const strings memory usage
#define SUPPORT_LOG_INFO
#if defined(SUPPORT_LOG_INFO)
    #define LOG(...) printf(__VA_ARGS__)
#else
    #define LOG(...)
#endif



//-----------------------------------------------------------------------------
// Global variables
//-----------------------------------------------------------------------------
static Game* game;



//------------------------------------------------------------------------
// Function definition
//------------------------------------------------------------------------
static void updateDrawFrame(void);



//-----------------------------------------------------------------------------
// Program main entry point
//-----------------------------------------------------------------------------
int main(void)
{
#if !defined(_DEBUG)
    SetTraceLogLevel(LOG_NONE);         // Disable raylib trace log messages
#endif

    game = new Game();

#if defined(PLATFORM_WEB)
    emscripten_set_main_loop(updateDrawFrame, 60, 1);
#else

    //-------------------------------------------------------------------------
    // Main game loop
    //-------------------------------------------------------------------------
    while (!WindowShouldClose())    // Detect window close button
    {
        updateDrawFrame();
    }
#endif

    delete game;

    return 0;
}



void updateDrawFrame(void)
{
    switch (game->current_window)
    {
    case GAME:
        game->drawGame();
        break;

    case PAUSE:
        game->drawPause();
        break;

    case PLAYGROUND:
        game->drawPlayground();
        break;

    case MAIN_MENU:
    default:
        game->drawMainMenu();
    }
}
