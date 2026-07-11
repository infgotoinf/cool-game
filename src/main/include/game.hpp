#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"

#include <map>
#include <vector>

#include "window_codes.hpp"



constexpr static float SECONDS_TO_CURSE = 20.0f;
constexpr static int FPS = 60;


static Vector2 getCursorPosFromCenter()
{
    return { static_cast<float>(360 - GetMouseX()), static_cast<float>(360 - GetMouseY()) };
}


enum ButtonStatus : unsigned short
{
    HOVERED
   ,NOT_HOVERED
};

enum GuyState : unsigned short
{
    WALK
   ,IDLE
};


struct Guy
{
    Vector2 pos;
    Color color;
    float speed;
    int frame;
    int state_change_cycles_left; ///< Animation cycles left till state changed
    GuyState state;
    bool is_moving_forward;

    Rectangle getHitbox() {
        return { pos.x + 30, pos.y + 16, 46, 120 }; // Hardcoding the hitboxes
    }
};


class Game
{
    std::map<const char*, std::vector<Texture2D>> animations;
    std::map<const char*, Texture2D> textures;
    std::map<const char*, Sound> sounds;
    std::vector<Texture2D> noise;
    std::vector<Guy> guys;
    Camera2D camera;
    Font font;
    RenderTexture2D target = { 0 };
    Texture2D vignette;
    float game_start_timestamp;
    float game_pause_timestamp;
    float curse_value; ///< Thing that drains in the game bar
    float curse_drain_speed;

    bool is_gameover;
    bool is_tutorial_over;

    constexpr static int screen_width = 720;
    constexpr static int screen_height = 720;

public:
    WindowID current_window = WindowID::MAIN_MENU;

    ///------------------------------------------------------------------------
    /// Game constructor. Loads textures, sets variables.
    ///------------------------------------------------------------------------
    Game();

    ///------------------------------------------------------------------------
    /// Game destructor. Frees memory.
    ///------------------------------------------------------------------------
    ~Game();

    ///------------------------------------------------------------------------
    /// Resets the game, just like then you restart the game. Sets variables to
    /// their defaults
    ///------------------------------------------------------------------------
    void resetGame();

    bool customButton(Texture2D& texture, int pos_x, int pos_y, ButtonStatus *past_status);

    ///------------------------------------------------------------------------
    /// Shakes screen.
    ///
    /// @param[in] shake_ammount How strong is the shake.
    /// @param[in] shake_supressor Value to gradually supress the screen shake.
    ///------------------------------------------------------------------------
    void shakeScreen(int shake_ammount, float shake_supressor);

    void regenCurse();

    void drainCurse();

    // Guy/guys related
    Guy createRandomGuy();

    void sortGuysByPosY();

    void drawGuys(int shift_x, int shift_y);

    // Screens
    void drawMainMenu();

    void drawGame();

    void drawPause();
};

#endif // GAME_HPP
