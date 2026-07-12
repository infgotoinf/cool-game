#ifndef GAME_HPP
#define GAME_HPP

#include "raylib.h"

#include <map>
#include <vector>

#include "window_codes.hpp"



constexpr static float SECONDS_TO_CURSE = 15.0f;
constexpr static int FPS = 60;


static Vector2 getCursorPosFromCenter()
{
    return { static_cast<float>(360 - GetMouseX()), static_cast<float>(360 - GetMouseY()) };
}



///----------------------------------------------------------------------------
/// Formats time to MM:SS;CC
///----------------------------------------------------------------------------
static const char* formatTime(float time)
{
    return TextFormat( "%02d:%02d;%02d"
                     , static_cast<int>(time / 60)
                     , static_cast<int>(time) % 60
                     , static_cast<int>(time * 100) % 100);
}


enum ObjectType : unsigned short
{
    NONE
   ,KNIFE
   ,DEAD
   ,ALIVE
   ,BOOK
};


enum EntityName : unsigned short
{
    NO
   ,SPIDER
   ,RAT
   ,BIRD
   ,LEAF
   ,NAIL
};


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


enum LayerZ : unsigned short
{
    FRONT
   ,ALMOST_FRONT
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

struct DragableObject
{
    std::vector<Texture2D> animation;
    Texture2D texture;
    Vector2 pos;
    unsigned hitbox_radius;
    ObjectType type;
    EntityName name;
    int state;
    bool hitbox_visible;
    int frame_counter = 0;
    int frame = 0;
};


struct Triangle {
    // Construcor values
    Vector2 point1;
    Vector2 point2;
    Vector2 point3;
    Color color;
    Color inactive_color;

    // Not constructor values
    bool is_active = false;
    DragableObject *object;

    Color GetColor();
};


class Game
{
    std::map<const char*, std::vector<Texture2D>> animations;
    std::map<const char*, Texture2D> textures;
    std::map<const char*, Sound> sounds;
    std::map<const char*, Music> music;
    std::vector<Texture2D> noise;
    std::vector<Guy> guys;
    std::vector<DragableObject> dragable_objects;
    Triangle hex_table_trinagles[6];
    DragableObject* hex_table_slots[6];
    DragableObject* dragged_object;
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
    bool is_dragging;
    bool is_holding_a_knife;

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
    ///------------------------------------------------------------------------
    void shakeScreen(int shake_ammount);

    void regenCurse();

    void drainCurse();

    void riseCurseDrainSpeed(int ammount);

    void drawHexTable(float x, float y, Color color);

    // Guy/guys related
    Guy createRandomGuy();

    void sortGuysByPosY();

    void drawGuys(int shift_x, int shift_y);

    void drawFace(int shift_x, int shift_y);

    Rectangle swingKnife();

    // Dragable Objects
    bool dragObjects();

    void drawDragableObjects(int shift_x, int shift_y, LayerZ pos_z);

    void createDragableObject(std::vector<Texture2D> animation, Texture2D texture, Vector2 position, unsigned radius, ObjectType type, EntityName name, int state = 0, bool hitbox_visible = false);

    // Screens
    void drawMainMenu();

    void drawGame();

    void drawPause();

    void drawGameOver();

    // Entities
    void updateEntities(Rectangle attack = { 0, 0, 0, 0 });

    void spawnSpider();

    void spawnBird();

    // void drawPlayground();
};

#endif // GAME_HPP
