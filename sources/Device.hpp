#pragma once

#include "Include.hpp"

namespace box
{
    class inputs_impl final : public inputs
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

    class video_impl final : public video
    {
    public:
        video_impl() = default;
        ~video_impl() override;

        bool is_window_fullscreen(void) override;
        bool is_window_hidden(void) override;
        bool is_window_minimized(void) override;
        bool is_window_maximized(void) override;
        bool is_window_focused(void) override;
        bool is_window_resized(void) override;
        bool is_window_state(unsigned int flag) override;
        void set_window_state(unsigned int flags) override;
        void clear_window_state(unsigned int flags) override;
        void toggle_fullscreen(void) override;
        void maximize_window(void) override;
        void minimize_window(void) override;
        void restore_window(void) override;


        void  set_window_title(const char* title) override;
        void  set_window_position(int x, int y) override;
        void  set_window_monitor(int monitor) override;
        void  set_window_minsize(int width, int height) override;
        void  set_window_size(int width, int height) override;
        void  set_window_opacity(float opacity) override;
        void* get_window_handle(void) override;
        int   get_screen_width(void) override;
        int   get_screen_height(void) override;
        int   get_render_width(void) override;
        int   get_render_height(void) override;

        Vec2i get_window_position(void) override;
        Vec2i get_window_scale_dpi(void) override;

        int         get_monitor_count(void) override;
        int         get_current_monitor(void) override;
        Vec2i       get_monitor_position(int monitor) override;
        int         get_monitor_width(int monitor) override;
        int         get_monitor_height(int monitor) override;
        int         get_monitor_physical_width(int monitor) override;
        int         get_monitor_physical_height(int monitor) override;
        int         get_monitor_refresh_rate(int monitor) override;
        const char* get_monitor_name(int monitor) override;
    };
} // namespace box