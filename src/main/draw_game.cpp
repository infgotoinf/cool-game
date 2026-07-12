#include "include/game.hpp"

#include "raylib.h"
#include "raymath.h"

#include <vector>

#include "window_codes.hpp"



static int animation_fps = 8;
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
//void drawHexTable(float x, float y, float size, Color color);


///----------------------------------------------------------------------------
/// Same as Raylibs DrawRectangle(), exept it draw rectangle up.
///----------------------------------------------------------------------------
void drawRectangleUp(float x, float y, float width, float height, Color color)
{
    DrawRectangle(x, y - height, width, height, color);
}


void drawCustomCursor(const std::vector<Texture2D> &animation)
{
    static int frame_counter = 0;
    static int frame = 0;

    ++frame_counter;
    if (frame_counter >= FPS / animation_fps)
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


Rectangle Game::swingKnife()
{
    static int frame_counter = 0;
    static int frame = 0;

    ++frame_counter;
    if (frame_counter >= FPS / animation_fps)
    {
        frame_counter = 0;
        ++frame;
        if (frame >= 3) frame = 0;
    }
    DrawTexture( animations["game_cursor_knife_swing"][frame]
               , static_cast<float>(GetMouseX() - animations["game_cursor_knife_swing"][frame].width * 0.5)
               , static_cast<float>(GetMouseY() - animations["game_cursor_knife_swing"][frame].height * 0.5)
               , WHITE);


    static float swing_size = 80;
    Rectangle rec = { (float) GetMouseX() - swing_size
                    , (float) GetMouseY() - swing_size / 2
                    , swing_size * 2, swing_size / 2 };
    // DrawRectangleRec(rec, RED);
    // DrawRectangleLinesEx(rec, 1, WHITE);
    return rec;
}


Rectangle getCursorHitbox()
{
    static float cursor_size = 60;
    Rectangle rec = { (float) GetMouseX() - cursor_size / 2
                    , (float) GetMouseY() - cursor_size / 2
                    , cursor_size, cursor_size };
    // DrawRectangleRec(rec, GOLD);
    // DrawRectangleLinesEx(rec, 1, WHITE);
    return rec;
}


bool Game::dragObjects()
{
    if (not is_dragging)
    {
        Rectangle cursor_hibox = getCursorHitbox();
        for (DragableObject& obj : dragable_objects)
        {
            if (CheckCollisionCircleRec(obj.pos, obj.hitbox_radius, cursor_hibox))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && obj.type != ALIVE && not is_holding_a_knife)
                {
                    is_dragging = true;
                    dragged_object = &obj;
                }
                else if (obj.type == KNIFE && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
                {
                    is_holding_a_knife = true;
                    obj.pos = { 1000, 1000}; // To lazy to delete this shit
                }
                return true;
            }
        }
    }
    else if (IsMouseButtonDown(MOUSE_BUTTON_LEFT) and is_dragging)
    {
        dragged_object->pos.x += GetMouseDelta().x;
        dragged_object->pos.y += GetMouseDelta().y;
    }
    else if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT) and is_dragging)
    {
        for (int i = 0; i < 6; ++i)
        {
            if (CheckCollisionPointTriangle(dragged_object->pos,
                hex_table_trinagles[i].point1,
                hex_table_trinagles[i].point2,
                hex_table_trinagles[i].point3))
            {
                hex_table_trinagles[i].color = WHITE;
                break;
            }
        }


        is_dragging = false;
        dragged_object = nullptr;
    }
    return false;
}


void Game::drawDragableObjects(int shift_x, int shift_y, LayerZ pos_z)
{
    for (DragableObject& obj : dragable_objects)
    {
        if ((obj.name == SPIDER || obj.name == BIRD) && obj.type == ALIVE && pos_z == FRONT) continue;
        if (not ((obj.name == SPIDER || obj.name == BIRD) && obj.type == ALIVE) && pos_z == ALMOST_FRONT) continue;

        if (obj.animation.empty())
            DrawTexture(obj.texture, obj.pos.x - obj.texture.width * 0.5 + shift_x, obj.pos.y - obj.texture.height * 0.5 + shift_y, WHITE);
        else
        {
            ++obj.frame_counter;
            if (obj.frame_counter >= FPS / animation_fps)
            {
                obj.frame_counter = 0;
                ++obj.frame;
                if (obj.frame >= 3) obj.frame = 0;
            }
            DrawTexture( obj.animation[obj.frame]
                       , obj.pos.x - obj.texture.width * 0.5 + shift_x, obj.pos.y - obj.texture.height * 0.5 + shift_y, WHITE);
        }

        if (obj.hitbox_visible)
        {
            DrawCircle(obj.pos.x + shift_x, obj.pos.y + shift_y, obj.hitbox_radius, { 120, 167, 210, 125 }); // Hardcoded debug hitbox color
        }
    }
}


void Game::spawnSpider()
{
    Vector2 rand_pos = { static_cast<float>(GetRandomValue(216, 72 * 7)), -36 };
    for (DragableObject &obj : dragable_objects)
    {
        if (obj.name == SPIDER && obj.type == DEAD)
        {
            obj.pos = rand_pos;
            obj.type = ALIVE;
            obj.state = 1;
        }
    }
    createDragableObject( animations["game_object_spider"], textures["game_object_spider"]
                        , rand_pos, 40, ALIVE, SPIDER, 1);
}


void Game::spawnBird()
{
    bool moves_right = GetRandomValue(0, 1);
    Vector2 rand_pos = { static_cast<float>(moves_right ? 72 : 72 * 9), static_cast<float>(GetRandomValue(72, 144))};
    std::vector<Texture2D> animation = animations[moves_right ? "game_object_bird_fly_back" : "game_object_bird_fly"];
    for (DragableObject &obj : dragable_objects)
        if (obj.name == BIRD && obj.type == DEAD)
        {
            obj.pos = rand_pos;
            obj.type = ALIVE;
            obj.state = moves_right ? 1 : -1;
            obj.animation = animation;
        }
    createDragableObject( animation, textures["game_object_bird"]
                        , rand_pos, 40, ALIVE, BIRD, 1);
}


void Game::updateEntities(Rectangle attack)
{
    for (DragableObject &obj : dragable_objects)
    {
        if (obj.name == SPIDER && obj.type == ALIVE)
        {
            static float pos_shift = 1.0f * curse_drain_speed;
            static int edge = 72 * 3;
            if (CheckCollisionCircleRec(obj.pos, obj.hitbox_radius, attack))
            {
                obj.name = NO;
                obj.type = DEAD;
                obj.state = 0;
                obj.animation = std::vector<Texture2D>();
            }
            else if (obj.pos.y <= edge && obj.state == 1)
            {
                obj.pos.y += pos_shift;
            }
            else if (obj.pos.y >= edge && obj.state == 1)
            {
                obj.pos.y -= pos_shift;
                obj.state = 2;
            }
            else if (obj.pos.y >= -36 && obj.state == 2)
            {
                obj.pos.y -= pos_shift;
            }
            else
            {
                obj.type = DEAD;
                obj.pos = { 1000, 1000 };
                obj.state = 0;
            }
        }
        else if (obj.name == BIRD && obj.type == ALIVE)
        {
            static float pos_shift = 2.0f * curse_drain_speed;
            static int edge = 72 * 4 + 10;
            static float edge2;
            if (CheckCollisionCircleRec(obj.pos, obj.hitbox_radius, attack))
            {
                obj.name = NO;
                obj.type = DEAD;
                obj.state = 0;
                obj.animation = std::vector<Texture2D>();
            }
            else if (obj.pos.y <= edge && obj.state == 1)
            {
                obj.pos.y += pos_shift;
                obj.pos.x += pos_shift;
            }
            else if (obj.pos.y <= edge && obj.state == -1)
            {
                obj.pos.y += pos_shift;
                obj.pos.x -= pos_shift;
            }
            else if (obj.pos.y >= edge && obj.state == 1)
            {
                obj.state = 2;
                edge2 = obj.pos.x + 1;
                obj.animation = animations["game_object_bird_idle_back"];
            }
            else if (obj.pos.y >= edge && obj.state == -1)
            {
                obj.state = -2;
                edge2 = obj.pos.x - 1;
                obj.animation = animations["game_object_bird_idle"];
            }
            else if (obj.pos.x <= edge2 && obj.state == 2)
            {
                obj.pos.x += pos_shift / 10;
            }
            else if (obj.pos.x >= edge2 && obj.state == -2)
            {
                obj.pos.x -= pos_shift / 10;
            }
            else if (obj.pos.x >= edge2 && obj.state == 2)
            {
                obj.state = 3;
                obj.animation = animations["game_object_bird_fly_back"];
            }
            else if (obj.pos.x <= edge2 && obj.state == -2)
            {
                obj.state = -3;
                obj.animation = animations["game_object_bird_fly"];
            }
            else if (obj.pos.x <= 720 - 72 && obj.state == 3)
            {
                obj.pos.x += pos_shift;
                obj.pos.y -= pos_shift;
            }
            else if (obj.pos.x >= 72 && obj.state == -3)
            {
                obj.pos.x -= pos_shift;
                obj.pos.y -= pos_shift;
            }
            else
            {
                obj.type = DEAD;
                obj.pos = { 1000, 1000 };
                obj.state = 0;
            }
        }
    }
}


void Game::createDragableObject(std::vector<Texture2D> animation, Texture2D texture, Vector2 position, unsigned radius, ObjectType type, EntityName name, int state, bool hitbox_visible)
{
    DragableObject obj = { animation, texture, position, radius, type, name, hitbox_visible};

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
    if (frame_counter >= FPS / animation_fps)
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
                    guy.speed = GetRandomValue(GUY_SPEED_SPREAD.x, GUY_SPEED_SPREAD.y) * 0.1 * curse_drain_speed;
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


void Game::drawFace(int shift_x, int shift_y)
{
    static int frame_counter = 0;
    static int frame = 0;
    ++frame_counter;
    if (frame_counter >= FPS / animation_fps)
    {
        frame_counter = 0;
        ++frame;
        if (frame >= 3) frame = 0;
    }
    DrawTexture( animations[curse_value >= SECONDS_TO_CURSE / 3 ? "game_face_calm" : "game_face_freakout"][frame]
               , 72 * 8 + shift_x, 72 * 8 + shift_y, WHITE);
}


void Game::shakeScreen(int shake_ammount)
{
    camera.offset = { 0, 0 };

    camera.offset.y += GetRandomValue(-shake_ammount, shake_ammount);
    camera.offset.x += GetRandomValue(-shake_ammount, shake_ammount);
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

    static float time = GetTime() - game_start_timestamp;
    if (time < GetTime() - game_start_timestamp)
    {
        ++time;
        curse_drain_speed += 0.01f;
    }

    if (curse_drain_speed <= 1.5f) animation_fps = 8;
    else if (curse_drain_speed <= 2.0f) animation_fps = 9;
    else if (curse_drain_speed <= 2.33f) animation_fps = 10;
    else if (curse_drain_speed <= 2.66f) animation_fps = 11;
    else if (curse_drain_speed <= 3.0f) animation_fps = 12;
    else if (curse_drain_speed <= 3.25f) animation_fps = 13;
    else if (curse_drain_speed <= 3.5f) animation_fps = 14;
    else if (curse_drain_speed <= 3.75f) animation_fps = 15;
    else if (curse_drain_speed <= 4.0f) animation_fps = 16;
    else if (curse_drain_speed <= 4.20f) animation_fps = 17;
    else if (curse_drain_speed <= 4.40f) animation_fps = 18;
    else if (curse_drain_speed <= 4.60f) animation_fps = 19;
    else if (curse_drain_speed <= 4.80f) animation_fps = 20;
    else if (curse_drain_speed <= 5.00f) animation_fps = 21;
    else if (curse_drain_speed >= 5.00f) animation_fps = 22;

    shakeScreen( curse_value <= SECONDS_TO_CURSE / 3
               ? SECONDS_TO_CURSE / 3 - curse_value : 0);


    static bool cursor_can_grab;

    if (is_holding_a_knife && IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
    {
        for (DragableObject &obj : dragable_objects)
        {
            if (obj.type == KNIFE)
            {
                obj.pos = GetMousePosition();
                break;
            }
        }
        is_holding_a_knife = false;
    }
    else
    {
        cursor_can_grab = dragObjects();
    }

    static int time_to_spawn_a_spider = game_start_timestamp + GetRandomValue(5 / curse_drain_speed, 15 / curse_drain_speed);
    if (time_to_spawn_a_spider < GetTime() - game_start_timestamp)
    {
        spawnSpider();
        time_to_spawn_a_spider = GetTime() - game_start_timestamp + GetRandomValue(5 / curse_drain_speed, 15 / curse_drain_speed);
    }

    static int time_to_spawn_a_bird = game_start_timestamp + GetRandomValue(5 / curse_drain_speed, 15 / curse_drain_speed);
    if (time_to_spawn_a_bird < GetTime() - game_start_timestamp)
    {
        spawnBird();
        time_to_spawn_a_bird = GetTime() - game_start_timestamp + GetRandomValue(5 / curse_drain_speed, 15 / curse_drain_speed);
    }

    // Draw
    //----------------------------------------------------------------------------------
    // Render game screen to a texture,
    // it could be useful for scaling or further shader postprocessing
    BeginTextureMode(target);
        BeginMode2D(camera);

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

            drawDragableObjects(getCursorPosFromCenter().x / 20, getCursorPosFromCenter().y / 20, ALMOST_FRONT);

            DrawTexture(textures["game_front"], -size_10th / 2 + getCursorPosFromCenter().x / 15
                                              , -size_10th / 2 + getCursorPosFromCenter().y / 15, WHITE);
            // DrawRectangle(72, 140, 578, 165, WHITE); // Draw guy walk area

            // Hex table
            drawHexTable(  getCursorPosFromCenter().x / 14
                        ,  getCursorPosFromCenter().y / 14
                        ,  BLACK);
            DrawTexture(textures["game_hex_table"], size_10th * 0.5f - 10.0f + getCursorPosFromCenter().x / 14
                                                  , size_10th         * 6.0f + getCursorPosFromCenter().y / 14
                                                  , WHITE);

            drawDragableObjects(getCursorPosFromCenter().x / 14, getCursorPosFromCenter().y / 14, FRONT);

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
                           , size_10th * 0.85, size_10th * 6.5 / SECONDS_TO_CURSE * curse_value, bar_color);
            DrawRectangle( size_10th * 8 + bar_paralax.x
                         , size_10th * 7.9 + bar_paralax.y
                         , size_10th * 1.85, size_10th * 1.8, bar_color);
            drawFace(bar_paralax.x, bar_paralax.y);
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
            DrawText(TextFormat("%d", time_to_spawn_a_spider), 10, 10, 24, PURPLE);

        EndMode2D();
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

        // Time
        DrawTextEx(font, formatTime(GetTime() - game_start_timestamp)
                       , { 90, 12 }
                       , 60, 1
                       , { 255, 255, 255, 50 });


        // Cursor
        if (is_holding_a_knife)
        {
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                updateEntities(swingKnife());
            }
            else
            {
                updateEntities();
                drawCustomCursor(animations["game_cursor_knife_hold"]);
            }
        }
        else
        {
            updateEntities();
            if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
            {
                drawCustomCursor(animations["game_cursor_grab"]);
            }
            else drawCustomCursor(cursor_can_grab ? animations["game_cursor_ready"] : animations["game_cursor_free"]);
        }
        getCursorHitbox();

    EndDrawing();
}


void Game::drawHexTable(float shift_x, float shift_y, Color color)
{
    //float width = 2 * size;
    //float height = sqrt(3) * size;



    // Top-left
    DrawTriangle( Vector2Add( hex_table_trinagles[0].point1, {shift_x, shift_y})
                , Vector2Add(hex_table_trinagles[0].point2, { shift_x, shift_y })
                , Vector2Add(hex_table_trinagles[0].point3, { shift_x, shift_y })
                , hex_table_trinagles[0].color );
    // Top
    DrawTriangle(Vector2Add(hex_table_trinagles[1].point1, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[1].point2, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[1].point3, { shift_x, shift_y })
        , hex_table_trinagles[1].color);
    // Top-right
    DrawTriangle(Vector2Add(hex_table_trinagles[2].point1, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[2].point2, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[2].point3, { shift_x, shift_y })
        , hex_table_trinagles[2].color);
    // Down-right
    DrawTriangle(Vector2Add(hex_table_trinagles[3].point1, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[3].point2, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[3].point3, { shift_x, shift_y })
        , hex_table_trinagles[3].color);
    // Down
    DrawTriangle(Vector2Add(hex_table_trinagles[4].point1, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[4].point2, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[4].point3, { shift_x, shift_y })
        , hex_table_trinagles[4].color);
    // Down-left
    DrawTriangle(Vector2Add(hex_table_trinagles[5].point1, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[5].point2, { shift_x, shift_y })
        , Vector2Add(hex_table_trinagles[5].point3, { shift_x, shift_y })
        , hex_table_trinagles[5].color);

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
