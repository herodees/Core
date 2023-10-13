#include "inputs.hpp"

namespace box
{
    inputs_impl::inputs_impl()
    {
    }

    inputs_impl::~inputs_impl()
    {
    }

    bool inputs_impl::is_key_pressed(int key) const
    {
        return ray::IsKeyPressed(key);
    }

    bool inputs_impl::is_key_down(int key) const
    {
        return ray::IsKeyDown(key);
    }

    bool inputs_impl::is_key_released(int key) const
    {
        return ray::IsKeyReleased(key);
    }

    bool inputs_impl::is_key_up(int key) const
    {
        return ray::IsKeyUp(key);
    }

    void inputs_impl::set_exit_key(int key)
    {
        return ray::SetExitKey(key);
    }

    int inputs_impl::get_key_pressed(void)
    {
        return ray::GetKeyPressed();
    }

    int inputs_impl::get_char_pressed(void)
    {
        return ray::GetCharPressed();
    }

    bool inputs_impl::is_gamepad_available(int gamepad)
    {
        return ray::IsGamepadAvailable(gamepad);
    }

    const char* inputs_impl::get_gamepad_name(int gamepad)
    {
        return ray::GetGamepadName(gamepad);
    }

    bool inputs_impl::is_gamepad_button_pressed(int gamepad, int button)
    {
        return ray::IsGamepadButtonPressed(gamepad, button);
    }

    bool inputs_impl::is_gamepad_button_down(int gamepad, int button)
    {
        return ray::IsGamepadButtonDown(gamepad, button);
    }

    bool inputs_impl::is_gamepad_button_released(int gamepad, int button)
    {
        return ray::IsGamepadButtonReleased(gamepad, button);
    }

    bool inputs_impl::is_gamepad_button_up(int gamepad, int button)
    {
        return ray::IsGamepadButtonUp(gamepad, button);
    }

    int inputs_impl::get_gamepad_button_pressed(void)
    {
        return ray::GetGamepadButtonPressed();
    }

    int inputs_impl::get_gamepad_axis_count(int gamepad)
    {
        return ray::GetGamepadAxisCount(gamepad);
    }

    float inputs_impl::get_gamepad_axis_movement(int gamepad, int axis)
    {
        return ray::GetGamepadAxisMovement(gamepad, axis);
    }

    int inputs_impl::set_gamepad_mappings(const char* mappings)
    {
        return ray::SetGamepadMappings(mappings);
    }

    bool inputs_impl::is_mouse_button_pressed(int button)
    {
        return ray::IsMouseButtonPressed(button);
    }

    bool inputs_impl::is_mouse_button_down(int button)
    {
        return ray::IsMouseButtonDown(button);
    }

    bool inputs_impl::is_mouse_button_released(int button)
    {
        return ray::IsMouseButtonReleased(button);
    }

    bool inputs_impl::is_mouse_button_up(int button)
    {
        return ray::IsMouseButtonUp(button);
    }

    int inputs_impl::get_mouse_x(void)
    {
        return ray::GetMouseX();
    }

    int inputs_impl::get_mouse_y(void)
    {
        return ray::GetMouseY();
    }

    Vec2i inputs_impl::get_mouse_position(void)
    {
        return ray::GetMousePosition();
    }

    Vec2i inputs_impl::get_mouse_delta(void)
    {
        return ray::GetMouseDelta();
    }

    void inputs_impl::set_mouse_position(int x, int y)
    {
        return ray::SetMousePosition(x,y);
    }

    void inputs_impl::set_mouse_offset(int offsetX, int offsetY)
    {
        return ray::SetMouseOffset(offsetX, offsetY);
    }

    void inputs_impl::set_mouse_scale(float scaleX, float scaleY)
    {
        return ray::SetMouseScale(scaleX, scaleY);
    }

    float inputs_impl::get_mouse_wheel_move(void)
    {
        return ray::GetMouseWheelMove();
    }

    Vec2i inputs_impl::get_mouse_wheel_move_v(void)
    {
        return ray::GetMouseWheelMoveV();
    }

    void inputs_impl::set_mouse_cursor(int cursor)
    {
        return ray::SetMouseCursor(cursor);
    }

    int inputs_impl::get_touch_x(void)
    {
        return ray::GetTouchX();
    }

    int inputs_impl::get_touch_y(void)
    {
        return ray::GetTouchY();
    }

    Vec2i inputs_impl::get_touch_position(int index)
    {
        return ray::GetTouchPosition(index);
    }

    int inputs_impl::get_touch_point_id(int index)
    {
        return ray::GetTouchPointId(index);
    }

    int inputs_impl::get_touch_point_count(void)
    {
        return ray::GetTouchPointCount();
    }
} // namespace box