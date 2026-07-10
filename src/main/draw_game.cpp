#include "include/game.hpp"

#include "raylib.h"
#include "raymath.h" // For Vector2Distance

#include <cmath>
#include <vector>

#include "window_codes.hpp"



constexpr static int ANIMATION_FPS = 10;
constexpr static Vector2 GUY_COLOR_SPREAD = { 0, 125 };
constexpr static Vector2 GUY_SPEED_SPREAD = { 5, 15 };
constexpr static Color BAR_COLOR100 = { 215, 0, 255, 255 }; // PURPLE
constexpr static Color BAR_COLOR0 = { 255, 0, 0, 255 }; // RED
constexpr static float CURSE_EXTREME_VALUE = 5.0f;


///----------------------------------------------------------------------------
/// Draws a 2D Hexidecimal shape in the center of [x, y] coordinate with given
/// size and color.
///
/// @note You can reed this to understand https://www.redblobgames.com/grids/hexagons/
///----------------------------------------------------------------------------
void drawHexTable(float x, float y, float size, Color color);


///----------------------------------------------------------------------------
/// Same as Raylibs DrawRectangle(), exept it draw rectangle up.
///----------------------------------------------------------------------------
void drawRectangleUp(float x, float y, float width, float height, Color color)
{
    DrawRectangle(x, y - height, width, height, color);
}


///----------------------------------------------------------------------------
/// Formats time to MM:SS;CC
///----------------------------------------------------------------------------
const char* formatTime(float time)
{
    return TextFormat( "%02d:%02d;%02d"
                     , static_cast<int>(time / 60)
                     , static_cast<int>(time) % 60
                     , static_cast<int>(time * 100) % 100);
}



void drawCustomCursor(const std::vector<Texture2D> &animation)
{
    static int frame_counter = 0;
    static int frame = 0;

    ++frame_counter;
    if (frame_counter >= ANIMATION_FPS)
    {
        frame_counter = 0;
        ++frame;
        if (frame >= animation.size()) frame = 0;
    }
    DrawTexture( animation[frame]
               , static_cast<float>(GetMouseX() - animation[frame].width * 0.5)
               , static_cast<float>(GetMouseY() - animation[frame].height * 0.5)
               , WHITE);
}


Rectangle getCursorHitbox()
{
    static float cursor_size = 60;
    Rectangle rec = { (float) GetMouseX() - cursor_size / 2
                    , (float) GetMouseY() - cursor_size / 2
                    , cursor_size, cursor_size };
    // DrawRectangleRec(rec, BLACK);
    // DrawRectangleLinesEx(rec, 1, WHITE);
    return rec;
}



void Game::dragObjects()
{
    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) and not is_dragging)
    {
        Rectangle cursor_hibox = getCursorHitbox();
        for (DragableObject& obj : dragable_objects)
        {
            if (CheckCollisionCircleRec(obj.pos, obj.hitbox_radius, cursor_hibox))
            {
                is_dragging = true;
                dragged_object = &obj;
                break;
            }
        }
    }
    else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) and is_dragging)
    {
        dragged_object->pos = GetMousePosition();
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) and is_dragging)
    {
        is_dragging = false;
        dragged_object = nullptr;
    }

}

void Game::drawDragableObjects()
{
    for (DragableObject obj : dragable_objects)
    {
        DrawRectangle(obj.pos.x, obj.pos.y, 40, 40, RED);
        if (obj.hitbox_visible)
        {
            DrawCircle(obj.pos.x, obj.pos.y, obj.hitbox_radius, { 120, 167, 210, 125 }); // Hardcoded debug hitbox color
        }
        //DrawTexture(obj.texture, obj.pos.x - obj.texture.width/2, obj.pos.y - obj.texture.height/2, WHITE);
    }
}

void Game::createDragableObject(Texture2D texture, Vector2 position, unsigned radius, bool hitbox_visible)
{
    DragableObject obj = { position, texture, radius, hitbox_visible};

    dragable_objects.push_back(obj);
}

void Game::sortGuysByPosY()
{
    for (int i = 0; i < guys.size(); ++i)
        for (int j = i + 1; j < guys.size(); ++j)
            if (guys[i].pos.y > guys[j].pos.y)
                std::swap(guys[i], guys[j]);
}


Guy Game::createRandomGuy()
{
    Vector2 y_pos_spread = { 140, 165 };
    Vector2 pos = { static_cast<float>(GetRandomValue(72, 578))
                  , static_cast<float>(GetRandomValue(y_pos_spread.x, y_pos_spread.y)) };
    // int random_color_skip1 = GetRandomValue(0, 2);
    // int random_color_skip2 = GetRandomValue(0, 2);
    // Color color = { static_cast<unsigned char>(random_color_skip1 == 0 || random_color_skip2 == 0 ? 0 : GetRandomValue(GUY_COLOR_SPREAD.x, GUY_COLOR_SPREAD.y))
    //               , static_cast<unsigned char>(random_color_skip1 == 1 || random_color_skip2 == 1 ? 0 : GetRandomValue(GUY_COLOR_SPREAD.x, GUY_COLOR_SPREAD.y))
    //               , static_cast<unsigned char>(random_color_skip1 == 2 || random_color_skip2 == 2 ? 0 : GetRandomValue(GUY_COLOR_SPREAD.x, GUY_COLOR_SPREAD.y)), 255 };
    // unsigned char c = GetRandomValue(GUY_COLOR_SPREAD.x, GUY_COLOR_SPREAD.y);
    unsigned char c = GUY_COLOR_SPREAD.y / (y_pos_spread.y - y_pos_spread.x) * (pos.y - y_pos_spread.x);
    Color color = { c, c, c, 255};
    float speed = GetRandomValue(GUY_SPEED_SPREAD.x, GUY_SPEED_SPREAD.y) * 0.1;
    GuyState state = GuyState::WALK;
    int frame = (state == WALK ? GetRandomValue(0, animations["game_guy_walk"].size())
                               : GetRandomValue(0, animations["game_guy_idle"].size()));
    int state_change_cycles_left = GetRandomValue(2, 20);
    bool is_moving_forward = GetRandomValue(0, 1);

    return Guy { pos, color, speed, frame, state_change_cycles_left, state, is_moving_forward };
}


void Game::drawGuys(int shift_x, int shift_y)
{
    HideCursor();

    static int frame_counter = 0;
    ++frame_counter;

    // Updating guys' frames, animations, values
    if (frame_counter >= FPS / ANIMATION_FPS)
    {
        for (Guy &guy : guys)
        {
            ++guy.frame;
            // frame, state_change_cycles_left change
            switch (guy.state)
            {
            case GuyState::WALK:
                if (guy.frame >= animations["game_guy_walk"].size())
                {
                    guy.frame = 0;
                    --guy.state_change_cycles_left;
                }
                break;

            case GuyState::IDLE:
            default:
                if (guy.frame >= animations["game_guy_idle"].size())
                {
                    guy.frame = 0;
                    --guy.state_change_cycles_left;
                }
                break;
            }

            // state, is_moving_forward, state_change_cycles_left change
            if (guy.state_change_cycles_left <= 0)
            {
                guy.state = (guy.state == WALK ? IDLE : WALK);
                // 33% chance to turn back
                bool was_moving_forward = guy.is_moving_forward;
                guy.is_moving_forward = (GetRandomValue(0, 2) ? guy.is_moving_forward
                                                              : !guy.is_moving_forward);
                guy.state_change_cycles_left = (guy.state == IDLE ? GetRandomValue(2, 20)
                                                                  : GetRandomValue(1, 10));
                if (was_moving_forward != guy.is_moving_forward)
                {
                    guy.speed = GetRandomValue(GUY_SPEED_SPREAD.x, GUY_SPEED_SPREAD.y) * 0.1;
                }
            }
        }
        frame_counter = 0;
    }

    for (Guy &guy : guys)
    {
        // Drawing a new guy if out of bounds
        if (guy.pos.x < 72 || guy.pos.x > 648)
        {
            guy = createRandomGuy();
            guy.pos.x = guy.is_moving_forward ? 72 : 648;
            sortGuysByPosY();
        }

        // Moving a walking guy
        if (guy.state == WALK) guy.pos.x += (static_cast<bool>(guy.is_moving_forward)
                                          ? guy.speed : -guy.speed);

        // Will draw guy's hitbox
        // DrawRectangleRec(guy.getHitbox(), GOLD);

        // Drawing guys
        DrawTexture( guy.state == WALK
                   ? (guy.is_moving_forward ? animations["game_guy_walk"][guy.frame]
                                            : animations["game_guy_walk_back"][guy.frame])
                   : (guy.is_moving_forward ? animations["game_guy_idle"][guy.frame]
                                            : animations["game_guy_idle_back"][guy.frame])
                   , guy.pos.x + shift_x, guy.pos.y + shift_y, guy.color);
    }
}


void Game::regenCurse() {
    curse_value = SECONDS_TO_CURSE;
}


void Game::drainCurse() {
    curse_value -= curse_drain_speed / FPS;
}


void Game::drawGame()
{
    if (!IsMusicStreamPlaying(music["inadequate"]))
    {
        PlayMusicStream(music["inadequate"]);
    }
    else
    {
        UpdateMusicStream(music["inadequate"]);
    }

    // https://github.com/Apfelstrudel-Technologien/raylibVignette/blob/main/main.c
    // static Shader vignette = LoadShader(0, "resources/vignette.fs");
    // static int rLoc = GetShaderLocation(vignette, "radius");
    // static int blurLoc = GetShaderLocation(vignette, "blur");
    // static int colLoc = GetShaderLocation(vignette, "color");

    drainCurse();
    if (curse_value <= 0) curse_value = SECONDS_TO_CURSE;

    Color bar_color = { static_cast<unsigned char>(( BAR_COLOR100.r * curse_value
                        + BAR_COLOR0.r * (SECONDS_TO_CURSE - curse_value)) / SECONDS_TO_CURSE)
                      , 0
                      , static_cast<unsigned char>(( BAR_COLOR100.b * curse_value
                        + BAR_COLOR0.b * (SECONDS_TO_CURSE - curse_value)) / SECONDS_TO_CURSE)
                      , 255 };

    Color curse_drain_speed_color =
                      { static_cast<unsigned char>(( BAR_COLOR0.r * (curse_drain_speed - 1)
                        + BAR_COLOR100.r * (CURSE_EXTREME_VALUE - curse_drain_speed)) / CURSE_EXTREME_VALUE)
                      , 0
                      , static_cast<unsigned char>(( BAR_COLOR0.b * (curse_drain_speed - 1)
                        + BAR_COLOR100.b * (CURSE_EXTREME_VALUE - (curse_drain_speed - 1))) / CURSE_EXTREME_VALUE)
                      , 255 };

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture,
    // it could be useful for scaling or further shader postprocessing
    BeginTextureMode(target);
        // ClearBackground(RAYWHITE);

        // In Krita I properly allign all those objects, but still we need to hardcode
        // a lot of values
        static float size_10th = screen_height * 0.1;

        // Interface base
        DrawTexture(textures["game_back"], size_10th + getCursorPosFromCenter().x / 50
                                         , getCursorPosFromCenter().y / 50, WHITE);
        DrawTexture(textures["game_middle"], size_10th + getCursorPosFromCenter().x / 40
                                           , getCursorPosFromCenter().y / 40, WHITE);

        drawGuys(getCursorPosFromCenter().x / 35, getCursorPosFromCenter().y / 35);

        DrawTexture(textures["game_almost_front"], size_10th + getCursorPosFromCenter().x / 20
                                                 , getCursorPosFromCenter().y / 20 + 10, WHITE);
        DrawTexture(textures["game_front"], -size_10th / 2 + getCursorPosFromCenter().x / 15
                                          , -size_10th / 2 + getCursorPosFromCenter().y / 15, WHITE);
        // DrawRectangle(72, 140, 578, 165, WHITE); // Draw guy walk area

        // Hex table
        drawHexTable( size_10th * 2.5 + 2 + getCursorPosFromCenter().x / 14
                    , size_10th * 8   - 5 + getCursorPosFromCenter().y / 14
                    , size_10th * 2 + 6, BLACK);
        DrawTexture(textures["game_hex_table"], size_10th * 0.5f - 10.0f + getCursorPosFromCenter().x / 14
                                              , size_10th         * 6.0f + getCursorPosFromCenter().y / 14
                                              , WHITE);

        DrawTexture(textures["game_curtains"], -size_10th / 2 + getCursorPosFromCenter().x / 12
                                             , -size_10th / 2 + getCursorPosFromCenter().y / 12, WHITE);

        const Vector2 bar_paralax = { getCursorPosFromCenter().x / 50, getCursorPosFromCenter().y / 50 };
        // Bar
        DrawRectangle( size_10th * 8.5 + bar_paralax.x
                     , size_10th * 1.55 + bar_paralax.y
                     , size_10th * 0.85, size_10th * 6.5, DARKGRAY);
        // Curse
        drawRectangleUp( size_10th * 8.5 + bar_paralax.x
                       , size_10th * 8.05 + bar_paralax.y
                       , size_10th * 0.85, size_10th * 6.5 / 20 * curse_value, bar_color);
        DrawRectangle( size_10th * 8 + bar_paralax.x
                     , size_10th * 7.9 + bar_paralax.y
                     , size_10th * 1.85, size_10th * 1.8, bar_color);
        DrawTexture(textures["game_bar_face"], size_10th * 8 + bar_paralax.x
                                             , size_10th * 8 + bar_paralax.y, WHITE);
        DrawTexture(textures["game_bar"], size_10th * 8   - 20 + bar_paralax.x
                                        , size_10th * 1.5 - 25 + bar_paralax.y, WHITE);

        // Curse drain speed
        DrawTextEx(font, TextFormat("%10.2fX", curse_drain_speed)
                       , { static_cast<float>( size_10th * 9
                                             - MeasureTextEx(font, TextFormat("%10.2fX", curse_drain_speed), 60, 1).x * 0.84)
                                             + bar_paralax.x
                       , size_10th / 3 * 2 + bar_paralax.y }
                       , 60, 1
                       , curse_drain_speed_color);

        // Time
        DrawTextEx(font, formatTime(GetTime() - game_start_timestamp)
                       , { 90, 12 }
                       , 60, 1
                       , { 255, 255, 255, 50 });

    EndTextureMode();

    // Pause
    static Color pause_color;
    if (CheckCollisionRecs(getCursorHitbox(), {18, 11, 40, 40}) || IsKeyPressed(KEY_ESCAPE))
    {
        pause_color = { 255, 255, 255, 100 };
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsKeyPressed(KEY_ESCAPE))
        {
            ShowCursor();
            PauseMusicStream(music["inadequate"]);
            PlaySound(sounds["button_click"]);
            current_window = WindowID::PAUSE;
            game_pause_timestamp = GetTime();
        }
    }
    else pause_color = { 255, 255, 255, 50 };

    // Render to screen (main framebuffer)
    BeginDrawing();

        // Draw render texture to screen, scaled if required
        DrawTexturePro(target.texture, { 0, 0, (float)target.texture.width, -(float)target.texture.height },
            { 0, 0, (float)target.texture.width, (float)target.texture.height }, { 0, 0 }, 0.0f, WHITE);

        // Draw effects
        DrawTexture(noise[static_cast<int>(GetTime() * 100) % noise.size()], 0, 0, {255, 255, 255, 20});
        // DrawTexture(vignette, 0, 0, { 255, 255, 255, 150});

        // Draw vignette
        // https://github.com/Apfelstrudel-Technologien/raylibVignette/blob/0795e875632cdd891a3f11b077bbe4da0ab176fc/main.c
        // BeginShaderMode(vignette);

        //     DrawTextureRec(v_texture.texture, { 0, 0
        //                                       , static_cast<float>(v_texture.texture.width)
        //                                       , static_cast<float>(-v_texture.texture.height) }, (Vector2){ 0, 0 }, BLANK);

        // EndShaderMode();

        // Pause
        // DrawRectangle(18, 11, 40, 40, WHITE); // Draw pause hitbox
        DrawRectangle(20,           11, 14, 38, pause_color);
        DrawRectangle(20 + 38 - 14, 11, 14, 38, pause_color);

        // Cursor
        drawCustomCursor(IsMouseButtonDown(MOUSE_LEFT_BUTTON) ? animations["game_cursor_grab"] : animations["game_cursor_free"]);
        getCursorHitbox();

    EndDrawing();
}


void drawHexTable(float x, float y, float size, Color color)
{
    float width = 2 * size;
    float height = sqrt(3) * size;

    // Top-left
    DrawTriangle( { x - width / 4, y - height / 2 }
                , { x - width / 2, y }
                , { x, y }
                , YELLOW );
    // Top
    DrawTriangle( { x - width / 4, y - height / 2 }
                , { x, y }
                , { x + width / 4, y - height / 2 }
                , GREEN );
    // Top-right
    DrawTriangle( { x + width / 4, y - height / 2 }
                , { x, y }
                , { x + width / 2, y }
                , RED );
    // Down-right
    DrawTriangle( { x, y }
                , { x + width / 4, y + height / 2 }
                , { x + width / 2, y }
                , PINK );
    // Down
    DrawTriangle( {x, y}
                , { x - width / 4, y + height / 2 }
                , { x + width / 4, y + height / 2 }
                , ORANGE );
    // Down-left
    DrawTriangle( { x, y }
                , { x - width / 2, y }
                , { x - width / 4, y + height / 2 }
                , SKYBLUE );

    // DrawCircle(x, y, 5, RED);
}

// void drawHexTable3D(float x, float y, float z, float size, Color color)
// {
//     float width = 2 * size;
//     float height = size;
//     float length = sqrt(3) * size;
//     Vector3 coordinats { x, y - height / 2, z };
//     size += (width - length) / sqrt(3);

//     DrawCylinder(coordinats, size, size, size, 6, color);
//     DrawCylinderWires(coordinats, size, size, size, 6, BLACK);
// }
