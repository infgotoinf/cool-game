#include "include/paralax_bg_example.hpp"

#include "raylib.h"

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void drawParalaxBgExample()
{
    // Initialization
    //--------------------------------------------------------------------------------------
    static int screenWidth = 720;
    static int screenHeight = 720;

    // NOTE: Be careful, background width must be equal or bigger than screen width
    // if not, texture should be draw more than two times for scrolling effect
    static Texture2D background = LoadTexture("resources/cyberpunk_street_background.png");
    static Texture2D midground = LoadTexture("resources/cyberpunk_street_midground.png");
    static Texture2D foreground = LoadTexture("resources/cyberpunk_street_foreground.png");

    static float scrollingBack = 0.0f;
    static float scrollingMid = 0.0f;
    static float scrollingFore = 0.0f;

    //--------------------------------------------------------------------------------------

    // Update
    //----------------------------------------------------------------------------------
    scrollingBack -= 0.1f;
    scrollingMid -= 0.5f;
    scrollingFore -= 1.0f;

    // NOTE: Texture is scaled twice its size, so it sould be considered on scrolling
    if (scrollingBack <= -background.width*2) scrollingBack = 0;
    if (scrollingMid <= -midground.width*2) scrollingMid = 0;
    if (scrollingFore <= -foreground.width*2) scrollingFore = 0;
    //----------------------------------------------------------------------------------

    // Draw
    //----------------------------------------------------------------------------------
    BeginDrawing();

        ClearBackground(GetColor(0x052c46ff));

        // Draw background image twice
        // NOTE: Texture is scaled twice its size
        DrawTextureEx(background, (Vector2){ scrollingBack, 20 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(background, (Vector2){ background.width*2 + scrollingBack, 20 }, 0.0f, 2.0f, WHITE);

        // Draw midground image twice
        DrawTextureEx(midground, (Vector2){ scrollingMid, 20 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(midground, (Vector2){ midground.width*2 + scrollingMid, 20 }, 0.0f, 2.0f, WHITE);

        // Draw foreground image twice
        DrawTextureEx(foreground, (Vector2){ scrollingFore, 70 }, 0.0f, 2.0f, WHITE);
        DrawTextureEx(foreground, (Vector2){ foreground.width*2 + scrollingFore, 70 }, 0.0f, 2.0f, WHITE);

        DrawText("BACKGROUND SCROLLING & PARALLAX", 10, 10, 20, RED);
        DrawText("(c) Cyberpunk Street Environment by Luis Zuno (@ansimuz)", screenWidth - 330, screenHeight - 20, 10, RAYWHITE);

    EndDrawing();
    //----------------------------------------------------------------------------------

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // UnloadTexture(background);  // Unload background texture
    // UnloadTexture(midground);   // Unload midground texture
    // UnloadTexture(foreground);  // Unload foreground texture
    //--------------------------------------------------------------------------------------
}
