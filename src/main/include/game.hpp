#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"

#include "window_codes.hpp"



class Game {
    Camera2D camera;
    RenderTexture2D target = { 0 };
    float game_start_timestamp;

    bool is_gameover;

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

    ///------------------------------------------------------------------------
    /// Shakes screen.
    ///
    /// @param[in] shake_ammount How strong is the shake.
    /// @param[in] shake_supressor Value to gradually supress the screen shake.
    ///------------------------------------------------------------------------
    void shakeScreen(int shake_ammount, float shake_supressor);

    ///------------------------------------------------------------------------
    ///------------------------------------------------------------------------
    // double showTime(double cur_time);

    ///------------------------------------------------------------------------
    /// Draws main menu screen.
    ///------------------------------------------------------------------------
    void drawMainMenu();

    ///------------------------------------------------------------------------
    /// Draws game screen.
    ///------------------------------------------------------------------------
    void drawGame();

    ///------------------------------------------------------------------------
    /// Draws pause screen.
    ///------------------------------------------------------------------------
    void drawPause();
};

#endif // GAME_HPP
