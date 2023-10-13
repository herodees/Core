#pragma once

#include "Include.hpp"

namespace box
{
    class inputs_impl : public inputs
    {
    public:
        inputs_impl();
        ~inputs_impl() override;

        bool        is_key_pressed(int key) const override;
        bool        is_key_down(int key) const override;
        bool        is_key_released(int key) const override;
        bool        is_key_up(int key) const override;
        void        set_exit_key(int key) override;
        int         get_key_pressed(void) override;
        int         get_char_pressed(void) override;
        bool        is_gamepad_available(int gamepad) override;
        const char* get_gamepad_name(int gamepad) override;
        bool        is_gamepad_button_pressed(int gamepad, int button) override;
        bool        is_gamepad_button_down(int gamepad, int button) override;
        bool        is_gamepad_button_released(int gamepad, int button) override;
        bool        is_gamepad_button_up(int gamepad, int button) override;
        int         get_gamepad_button_pressed(void) override;
        int         get_gamepad_axis_count(int gamepad) override;
        float       get_gamepad_axis_movement(int gamepad, int axis) override;
        int         set_gamepad_mappings(const char* mappings) override;
        bool        is_mouse_button_pressed(int button) override;
        bool        is_mouse_button_down(int button) override;
        bool        is_mouse_button_released(int button) override;
        bool        is_mouse_button_up(int button) override;
        int         get_mouse_x(void) override;
        int         get_mouse_y(void) override;
        Vec2i       get_mouse_position(void) override;
        Vec2i       get_mouse_delta(void) override;
        void        set_mouse_position(int x, int y) override;
        void        set_mouse_offset(int offsetX, int offsetY) override;
        void        set_mouse_scale(float scaleX, float scaleY) override;
        float       get_mouse_wheel_move(void) override;
        Vec2i       get_mouse_wheel_move_v(void) override;
        void        set_mouse_cursor(int cursor) override;
        int         get_touch_x(void) override;
        int         get_touch_y(void) override;
        Vec2i       get_touch_position(int index) override;
        int         get_touch_point_id(int index) override;
        int         get_touch_point_count(void) override;
    };
} // namespace box