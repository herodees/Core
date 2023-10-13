#pragma once

namespace box
{
    class inputs
    {
    public:
        virtual ~inputs() = default;

        // Input-related functions: keyboard
        virtual bool is_key_pressed(int key) const  = 0; // Check if a key has been pressed once
        virtual bool is_key_down(int key) const     = 0; // Check if a key is being pressed
        virtual bool is_key_released(int key) const = 0; // Check if a key has been released once
        virtual bool is_key_up(int key) const       = 0; // Check if a key is NOT being pressed
        virtual void set_exit_key(int key)          = 0; // Set a custom key to exit program (default is ESC)
        virtual int get_key_pressed(void) = 0; // Get key pressed (keycode), call it multiple times for keys queued, returns 0 when the queue is empty
        virtual int get_char_pressed(void) = 0; // Get char pressed (unicode), call it multiple times for chars queued, returns 0 when the queue is empty

        // Input-related functions: gamepads
        virtual bool        is_gamepad_available(int gamepad) = 0;           // Check if a gamepad is available
        virtual const char* get_gamepad_name(int gamepad)     = 0;           // Get gamepad internal name id
        virtual bool is_gamepad_button_pressed(int gamepad, int button) = 0; // Check if a gamepad button has been pressed once
        virtual bool is_gamepad_button_down(int gamepad, int button) = 0; // Check if a gamepad button is being pressed
        virtual bool is_gamepad_button_released(int gamepad, int button) = 0; // Check if a gamepad button has been released once
        virtual bool is_gamepad_button_up(int gamepad, int button) = 0; // Check if a gamepad button is NOT being pressed
        virtual int get_gamepad_button_pressed(void)    = 0;            // Get the last gamepad button pressed
        virtual int get_gamepad_axis_count(int gamepad) = 0;            // Get gamepad axis count for a gamepad
        virtual float get_gamepad_axis_movement(int gamepad, int axis) = 0; // Get axis movement value for a gamepad axis
        virtual int set_gamepad_mappings(const char* mappings) = 0; // Set internal gamepad mappings (SDL_GameControllerDB)

        // input-related functions: mouse
        virtual bool  is_mouse_button_pressed(int button)         = 0; // Check if a mouse button has been pressed once
        virtual bool  is_mouse_button_down(int button)            = 0; // Check if a mouse button is being pressed
        virtual bool  is_mouse_button_released(int button)        = 0; // Check if a mouse button has been released once
        virtual bool  is_mouse_button_up(int button)              = 0; // Check if a mouse button is NOT being pressed
        virtual int   get_mouse_x(void)                           = 0; // Get mouse position X
        virtual int   get_mouse_y(void)                           = 0; // Get mouse position Y
        virtual Vec2i get_mouse_position(void)                    = 0; // Get mouse position XY
        virtual Vec2i get_mouse_delta(void)                       = 0; // Get mouse delta between frames
        virtual void  set_mouse_position(int x, int y)            = 0; // Set mouse position XY
        virtual void  set_mouse_offset(int offsetX, int offsetY)  = 0; // Set mouse offset
        virtual void  set_mouse_scale(float scaleX, float scaleY) = 0; // Set mouse scaling
        virtual float get_mouse_wheel_move(void)   = 0; // Get mouse wheel movement for X or Y, whichever is larger
        virtual Vec2i get_mouse_wheel_move_v(void) = 0; // Get mouse wheel movement for both X and Y
        virtual void  set_mouse_cursor(int cursor) = 0; // Set mouse cursor

        // input-related functions: touch
        virtual int get_touch_x(void) = 0; // Get touch position X for touch point 0 (relative to screen size)
        virtual int get_touch_y(void) = 0; // Get touch position Y for touch point 0 (relative to screen size)
        virtual Vec2i get_touch_position(int index) = 0; // Get touch position XY for a touch point index (relative to screen size)
        virtual int get_touch_point_id(int index) = 0; // Get touch point identifier for given index
        virtual int get_touch_point_count(void)   = 0;
    };
} // namespace box