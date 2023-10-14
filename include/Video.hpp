#pragma once

namespace box
{
    class video
    {
    public:
        virtual ~video() = default;

        virtual bool is_window_fullscreen(void) = 0; // Check if window is currently fullscreen
        virtual bool is_window_hidden(void)     = 0; // Check if window is currently hidden (only PLATFORM_DESKTOP)
        virtual bool is_window_minimized(void)  = 0; // Check if window is currently minimized (only PLATFORM_DESKTOP)
        virtual bool is_window_maximized(void)  = 0; // Check if window is currently maximized (only PLATFORM_DESKTOP)
        virtual bool is_window_focused(void)    = 0; // Check if window is currently focused (only PLATFORM_DESKTOP)
        virtual bool is_window_resized(void)    = 0; // Check if window has been resized last frame
        virtual bool is_window_state(unsigned int flag) = 0; // Check if one specific window flag is enabled
        virtual void set_window_state(unsigned int flags) = 0; // Set window configuration state using flags (only PLATFORM_DESKTOP)
        virtual void clear_window_state(unsigned int flags) = 0; // Clear window configuration state flags
        virtual void toggle_fullscreen(void) = 0; // Toggle window state: fullscreen/windowed (only PLATFORM_DESKTOP)
        virtual void maximize_window(void)   = 0; // Set window state: maximized, if resizable (only PLATFORM_DESKTOP)
        virtual void minimize_window(void)   = 0; // Set window state: minimized, if resizable (only PLATFORM_DESKTOP)
        virtual void restore_window(void)    = 0; // Set window state: not minimized/maximized (only PLATFORM_DESKTOP)


        virtual void set_window_title(const char* title) = 0; // Set title for window (only PLATFORM_DESKTOP)
        virtual void set_window_position(int x, int y)   = 0; // Set window position on screen (only PLATFORM_DESKTOP)
        virtual void set_window_monitor(int monitor)     = 0; // Set monitor for the current window (fullscreen mode)
        virtual void set_window_minsize(int width, int height) = 0; // Set window minimum dimensions (for FLAG_WINDOW_RESIZABLE)
        virtual void  set_window_size(int width, int height) = 0; // Set window dimensions
        virtual void  set_window_opacity(float opacity) = 0; // Set window opacity [0.0f..1.0f] (only PLATFORM_DESKTOP)
        virtual void* get_window_handle(void)           = 0; // Get native window handle
        virtual int   get_screen_width(void)            = 0; // Get current screen width
        virtual int   get_screen_height(void)           = 0; // Get current screen height
        virtual int   get_render_width(void)            = 0; // Get current render width (it considers HiDPI)
        virtual int   get_render_height(void)           = 0; // Get current render height (it considers HiDPI)

        virtual Vec2i get_window_position(void)  = 0; // Get window position XY on monitor
        virtual Vec2i get_window_scale_dpi(void) = 0; // Get window scale DPI factor

        virtual int   get_monitor_count(void)           = 0; // Get number of connected monitors
        virtual int   get_current_monitor(void)         = 0; // Get current connected monitor
        virtual Vec2i get_monitor_position(int monitor) = 0; // Get specified monitor position
        virtual int get_monitor_width(int monitor) = 0; // Get specified monitor width (current video mode used by monitor)
        virtual int get_monitor_height(int monitor) = 0; // Get specified monitor height (current video mode used by monitor)
        virtual int get_monitor_physical_width(int monitor) = 0;  // Get specified monitor physical width in millimetres
        virtual int get_monitor_physical_height(int monitor) = 0; // Get specified monitor physical height in millimetres
        virtual int get_monitor_refresh_rate(int monitor) = 0;    // Get specified monitor refresh rate
        virtual const char* get_monitor_name(int monitor) = 0; // Get the human-readable, UTF-8 encoded name of the specified monitor
    };

} // namespace box