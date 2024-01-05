#pragma once

#include "Include.hpp"

namespace box
{
    struct string_dialog : imgui_dialog
    {
        string_dialog(std::string_view title = "Rename");

        void        show() override;
        virtual bool validate(std::string_view value);
        static int  text_callback(ImGuiInputTextCallbackData* data);

        std::string _title_str;
        std::string _value;
    };


    class imgui_impl final : public imgui
    {
    public:
        imgui_impl();
        ~imgui_impl() override;

        void same_line();
        bool button(const char* label, Vec2f size = {0, 0});
        bool input_int(const char* label, int32_t& value, int32_t step = 1, int32_t step_fast = 100);
        bool input_float(const char* label, float& value, float step = 0.f, float step_fast = 0.f);
        bool input_text(const char* label, imgui_buffer& string);
        bool begin_combobox(const char* label, const char* value);
        void end_combobox();
        bool selectable(const char* label, bool selected);
        bool begin_frame(const char* label, Vec2f size = {0, 0});
        void end_frame();
        bool collapsing_header(const char* label, bool* opened = nullptr);
        void show_dialog(imgui_dialog* dialog);

    public:
        bool is_active() const override;
        void init(bool dark_theme);
        void deinit();
        void on_frame_begin() override;
        void on_frame_end() override;

    private:

        imgui_dialog*              _open_dialog{};
        std::vector<imgui_dialog*> _dialogs;
        bool _active{true};
    };
} // namespace box