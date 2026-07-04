#include "include/python_vc_cpp.hpp"

#include "raylib.h"

#include <cmath>
#include <cstring>


class Game {
    Camera2D camera;
    Texture2D python_tex;
    Texture2D cpp_tex;

    Vector2 player_pos;
    Vector2 player_velocity;
    Vector2 cpp_guy_pos;
    Vector2 cpp_guy_velocity;
    constexpr static int image_size = 100;
    constexpr static float speed = 5;
    float ground_y_border;
    float time;

public:
    bool gameover;

    void resetGame() {
        player_pos =  { static_cast<float>(GetScreenWidth()) / 4, ground_y_border - image_size };
        cpp_guy_pos = { static_cast<float>(GetScreenWidth())
                      , ground_y_border - image_size };

        camera = { 0 };
        camera.target = (Vector2){ player_pos.x, 0 };
        camera.offset = (Vector2){ 0, 0 };
        camera.rotation = 0.0f;
        camera.zoom = 1.0f;

        player_velocity = { 0, 0 };
        cpp_guy_velocity = { 0, 0 };

        gameover = false;

        time = GetTime();
    }

    Game() {
        Image python_img = LoadImage("resources/python-guy.png");
        ImageResize(&python_img, image_size, image_size);
        python_tex = LoadTextureFromImage(python_img);
        UnloadImage(python_img);

        Image cpp_img = LoadImage("resources/cpp-guy.png");
        ImageResize(&cpp_img, image_size, image_size);
        cpp_tex = LoadTextureFromImage(cpp_img);
        UnloadImage(cpp_img);

        ground_y_border = static_cast<float>(GetScreenHeight()) / 5 * 4;

        resetGame();
    }

    //-----------------------------------------------------------------------------------
    // Change player pos
    //-----------------------------------------------------------------------------------
    void movePlayerLeft() {
        player_velocity.x += speed / 10;
    }

    void movePlayerRight() {
        player_velocity.x -= speed / 10;
    }

    void movePlayerDown() {
        player_velocity.y -= speed / 2;
    }

    void movePlayerUp() {
        player_velocity.y += speed * speed;
    }

    void updatePlayerPos() {
        player_pos.y -= player_velocity.y--;
        player_pos.x -= player_velocity.x;

        if (player_pos.y > ground_y_border - image_size) {
            player_pos.y = ground_y_border - image_size;
            player_velocity.y = 0;
        }
        else if (player_pos.y < 0) {
            player_pos.y = 0;
            player_velocity.y = 0;
        }

        if (player_pos.x > GetScreenWidth() - static_cast<float>(image_size) / 3 * 2) {
            player_pos.x = GetScreenWidth() - static_cast<float>(image_size) / 3 * 2;
            player_velocity.x = 0;
        }

        else if (player_pos.x < - static_cast<float>(image_size) / 3) {
            player_pos.x = - static_cast<float>(image_size) / 3;
            player_velocity.x = 0;
        }
    }

    //-----------------------------------------------------------------------------------
    // Change cpp guy pos
    //-----------------------------------------------------------------------------------
    void moveCppGuyLeft() {
        cpp_guy_velocity.x += speed / 5;
    }

    void moveCppGuyRight() {
        cpp_guy_velocity.x -= speed / 5;
    }

    void moveCppGuyUp() {
        cpp_guy_velocity.y += speed * speed * 2;
    }

    void updateCppGuyVelocity() {
        if (player_pos.x < cpp_guy_pos.x)
            moveCppGuyLeft();
        else
            moveCppGuyRight();

        if (player_pos.y < cpp_guy_pos.y)
            moveCppGuyUp();
    }

    void updateCppGuyPos() {
        cpp_guy_pos.y -= cpp_guy_velocity.y--;
        cpp_guy_pos.x -= cpp_guy_velocity.x;

        if (cpp_guy_pos.y > ground_y_border - image_size) {
            cpp_guy_pos.y = ground_y_border - image_size;
            cpp_guy_velocity.y = 0;
        }
        else if (cpp_guy_pos.y < 0) {
            cpp_guy_pos.y = 0;
            cpp_guy_velocity.y = 0;
        }

        if (cpp_guy_pos.x > GetScreenWidth() - static_cast<float>(image_size) / 3 * 2) {
            cpp_guy_pos.x = GetScreenWidth() - static_cast<float>(image_size) / 3 * 2;
            cpp_guy_velocity.x = 0;
        }

        else if (cpp_guy_pos.x < - static_cast<float>(image_size) / 3) {
            cpp_guy_pos.x = - static_cast<float>(image_size) / 3;
            cpp_guy_velocity.x = 0;
        }
    }

    //-----------------------------------------------------------------------------------
    //-----------------------------------------------------------------------------------
    void updateCameraTarget() {
        camera.target = (Vector2){ 0, 0 };
    }

    bool checkIfGuysCollide() {
        if (std::abs(cpp_guy_pos.x - player_pos.x) < static_cast<float>(image_size) / 3 * 2 &&
            std::abs(cpp_guy_pos.y - player_pos.y) < image_size)
            return true;
        return false;
    }

    void shakeScreen() {
        camera.offset = (Vector2){ 0, 0 };
        camera.rotation = 0.0f;

        constexpr static int shake = 2;
        int spread = std::pow((GetScreenWidth() - (cpp_guy_pos.x - player_pos.x)
                             + GetScreenHeight() - (cpp_guy_pos.y - player_pos.y)), shake) / std::pow(1000, shake);

        camera.offset.y += GetRandomValue(-spread, spread);
        camera.offset.x += GetRandomValue(-spread, spread);
        // camera.rotation += static_cast<float>(GetRandomValue(-spread, spread)) / 10;
    }

    void drawGame() {
        ClearBackground(DARKGRAY);

        BeginMode2D(camera);

            DrawRectangle(-GetScreenWidth() / 2, ground_y_border
                         , GetScreenWidth() * 2, ground_y_border, RAYWHITE);

            DrawTextureV( python_tex, player_pos, WHITE);
            DrawTextureV( cpp_tex, cpp_guy_pos, WHITE);

        EndMode2D();
    }

    double showTime(double cur_time) {
        double ret_time = cur_time - time;
        DrawText(TextFormat("%06.3f", ret_time)
               , 30, 30, 20, WHITE);
        return ret_time;
    }
};

//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
void drawPythonVcCpp()
{
    // Initialization
    //--------------------------------------------------------------------------------------

    static  Game game = Game();

    // Update
    //----------------------------------------------------------------------------------
    // Player movement

    if (!game.checkIfGuysCollide() && !game.gameover) {
        if (IsKeyDown(KEY_DOWN)  | IsKeyDown(KEY_J) | IsKeyDown(KEY_S)) game.movePlayerDown();
        if (IsKeyPressed(KEY_UP) | IsKeyPressed(KEY_K) | IsKeyPressed(KEY_W)) game.movePlayerUp();
        if (IsKeyDown(KEY_LEFT)  | IsKeyDown(KEY_H) | IsKeyDown(KEY_A)) game.movePlayerLeft();
        if (IsKeyDown(KEY_RIGHT) | IsKeyDown(KEY_L) | IsKeyDown(KEY_D)) game.movePlayerRight();
    }
    else {
        game.gameover = true;
        if (IsKeyPressed(KEY_ENTER))
            game.resetGame();
    }

    game.updatePlayerPos();
    game.updateCppGuyVelocity();
    game.updateCppGuyPos();

    game.updateCameraTarget();
    game.shakeScreen();

    //----------------------------------------------------------------------------------
    // Draw
    //----------------------------------------------------------------------------------

    BeginDrawing();

        game.drawGame();

        static double time;
        if (!game.gameover)
            time = game.showTime(GetTime());

        if (game.gameover) {
            constexpr static int text_size = 80;
            // constexpr static const char* gameover_text = "GAMEOVER";
            // DrawText(gameover_text
            //         , GetScreenWidth() / 2 - text_size * (strlen(gameover_text) / 2)
            //         , GetScreenHeight() / 2 - text_size / 2
            //         , text_size, RED);

            constexpr static const char* win_text = "YOU WIN";
            DrawText(win_text
                    , GetScreenWidth() / 2 - text_size * (strlen(win_text) / 2)
                    , GetScreenHeight() / 2 - text_size / 2
                    , text_size, GREEN);

            constexpr static int small_text_size = 20;
            constexpr static const char* guide_text = "PRESS [ENTER] TO TRY AGAIN";
            DrawText(guide_text
                    , GetScreenWidth() / 2 - small_text_size * (strlen(guide_text) / 2)
                    , GetScreenHeight() / 2 - small_text_size / 2 + text_size / 3 * 2
                    , small_text_size, LIGHTGRAY);

            DrawText(TextFormat("Time: %06.3f", time)
                    , GetScreenWidth() / 2 - small_text_size
                    , GetScreenHeight() / 2 - small_text_size / 2 + text_size
                    , small_text_size, LIGHTGRAY);
        }

    EndDrawing();
}
