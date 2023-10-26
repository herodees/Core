#include "Imgui.hpp"

#define NO_FONT_AWESOME
namespace ray
{
    #include <imgui_impl_raylib.h>
    #include <rlImGui.cpp>
}


namespace box
{
    imgui_impl::imgui_impl()
    {
    }

    imgui_impl::~imgui_impl()
    {
    }

    void imgui_impl::same_line()
    {
        ImGui::SameLine();
    }

    bool imgui_impl::button(const char* label, Vec2f size)
    {
        return ImGui::Button(label, (const ImVec2&)size);
    }

    bool imgui_impl::input_int(const char* label, int32_t& value, int32_t step, int32_t step_fast)
    {
        return ImGui::InputInt(label, &value, step, step_fast);
    }

    bool imgui_impl::input_float(const char* label, float& value, float step, float step_fast)
    {
        return ImGui::InputFloat(label, &value, step, step_fast);
    }

    bool imgui_impl::input_text(const char* label, string_buffer& string)
    {
        return ImGui::InputText(label, 0, 0);
    }

    bool imgui_impl::begin_combobox(const char* label, const char* value)
    {
        return ImGui::BeginCombo(label, value);
    }

    void imgui_impl::end_combobox()
    {
        ImGui::EndCombo();
    }

    bool imgui_impl::selectable(const char* label, bool selected)
    {
        return ImGui::Selectable(label, selected);
    }

    bool imgui_impl::begin_frame(const char* label, Vec2f size)
    {
        return ImGui::BeginChildFrame(ImGui::GetID(label), (const ImVec2&)size);
    }

    void imgui_impl::end_frame()
    {
        ImGui::EndChildFrame();
    }

    bool imgui_impl::collapsing_header(const char* label, bool* opened)
    {
        return ImGui::CollapsingHeader(label, opened);
    }

    void imgui_impl::show_dialog(imgui_dialog* dialog)
    {
    }

    bool imgui_impl::is_active() const
    {
        return _active;
    }

    void imgui_impl::init(bool dark_theme)
    {
        ray::rlImGuiSetup(dark_theme);
    }

    void imgui_impl::deinit()
    {
        ray::rlImGuiShutdown();
    }

    void imgui_impl::on_frame_begin()
    {
        ray::rlImGuiBegin();
    }

    void imgui_impl::on_frame_end()
    {
        ImGui::ShowDemoWindow();

        ray::rlImGuiEnd();
    }


} // namespace box