#pragma once

namespace box
{
    struct imgui_buffer
    {
        virtual ~imgui_buffer()  = 0;
        virtual char*  data()    = 0;
        virtual size_t size()    = 0;
        virtual size_t resize()  = 0;
    };

    struct imgui_dialog
    {
        imgui_dialog()          = default;
        virtual ~imgui_dialog() = default;
        virtual void show()     = 0;

        bool        _closed{};
        const char* _title{};
    };

    class imgui
    {
    public:
        virtual ~imgui() = default;

        virtual void show_dialog(imgui_dialog* dialog) = 0;

        virtual bool is_active() const = 0;
        virtual void on_frame_begin()  = 0;
        virtual void on_frame_end()    = 0;
    };
} // namespace box