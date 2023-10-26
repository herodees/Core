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

        ImGui::GetIO().IniFilename = nullptr;

        // Cherry style from ImThemes
        ImGuiStyle& style = ImGui::GetStyle();

        style.Alpha                            = 1.0f;
        style.DisabledAlpha                    = 0.6000000238418579f;
        style.WindowPadding                    = ImVec2(3.0f, 3.0f);
        style.WindowRounding                   = 0.0f;
        style.WindowBorderSize                 = 1.0f;
        style.WindowMinSize                    = ImVec2(32.0f, 32.0f);
        style.WindowTitleAlign                 = ImVec2(0.5f, 0.5f);
        style.WindowMenuButtonPosition         = ImGuiDir_Left;
        style.ChildRounding                    = 0.0f;
        style.ChildBorderSize                  = 1.0f;
        style.PopupRounding                    = 0.0f;
        style.PopupBorderSize                  = 1.0f;
        style.FramePadding                     = ImVec2(3.0f, 3.0f);
        style.FrameRounding                    = 3.0f;
        style.FrameBorderSize                  = 1.0f;
        style.ItemSpacing                      = ImVec2(3.0f, 3.0f);
        style.ItemInnerSpacing                 = ImVec2(3.0f, 3.0f);
        style.CellPadding                      = ImVec2(3.0f, 3.0f);
        style.IndentSpacing                    = 21.0f;
        style.ColumnsMinSpacing                = 3.0f;
        style.ScrollbarSize                    = 13.0f;
        style.ScrollbarRounding                = 3.0f;
        style.GrabMinSize                      = 20.0f;
        style.GrabRounding                     = 3.0f;
        style.TabRounding                      = 3.0f;
        style.TabBorderSize                    = 1.0f;
        style.TabMinWidthForCloseButton        = 0.0f;
        style.ColorButtonPosition              = ImGuiDir_Right;
        style.ButtonTextAlign                  = ImVec2(0.5f, 0.5f);
        style.SelectableTextAlign              = ImVec2(0.0f, 0.0f);
        ImVec4* colors                         = ImGui::GetStyle().Colors;
        colors[ImGuiCol_Text]                  = ImVec4(0.86f, 0.93f, 0.89f, 0.88f);
        colors[ImGuiCol_TextDisabled]          = ImVec4(0.86f, 0.93f, 0.89f, 0.28f);
        colors[ImGuiCol_WindowBg]              = ImVec4(0.13f, 0.14f, 0.17f, 1.00f);
        colors[ImGuiCol_ChildBg]               = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_PopupBg]               = ImVec4(0.20f, 0.22f, 0.27f, 0.90f);
        colors[ImGuiCol_Border]                = ImVec4(0.54f, 0.48f, 0.25f, 0.16f);
        colors[ImGuiCol_BorderShadow]          = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_FrameBg]               = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        colors[ImGuiCol_FrameBgHovered]        = ImVec4(0.45f, 0.20f, 0.30f, 0.78f);
        colors[ImGuiCol_FrameBgActive]         = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_TitleBg]               = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
        colors[ImGuiCol_TitleBgActive]         = ImVec4(0.26f, 0.08f, 0.16f, 0.52f);
        colors[ImGuiCol_TitleBgCollapsed]      = ImVec4(0.20f, 0.22f, 0.27f, 0.75f);
        colors[ImGuiCol_MenuBarBg]             = ImVec4(0.20f, 0.22f, 0.27f, 0.47f);
        colors[ImGuiCol_ScrollbarBg]           = ImVec4(0.20f, 0.22f, 0.27f, 1.00f);
        colors[ImGuiCol_ScrollbarGrab]         = ImVec4(0.09f, 0.15f, 0.16f, 1.00f);
        colors[ImGuiCol_ScrollbarGrabHovered]  = ImVec4(0.45f, 0.20f, 0.30f, 0.78f);
        colors[ImGuiCol_ScrollbarGrabActive]   = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_CheckMark]             = ImVec4(0.38f, 0.65f, 0.91f, 1.00f);
        colors[ImGuiCol_SliderGrab]            = ImVec4(0.38f, 0.65f, 0.91f, 1.00f);
        colors[ImGuiCol_SliderGrabActive]      = ImVec4(0.11f, 0.52f, 1.00f, 1.00f);
        colors[ImGuiCol_Button]                = ImVec4(0.47f, 0.77f, 0.83f, 0.14f);
        colors[ImGuiCol_ButtonHovered]         = ImVec4(0.45f, 0.20f, 0.30f, 0.86f);
        colors[ImGuiCol_ButtonActive]          = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_Header]                = ImVec4(0.45f, 0.20f, 0.30f, 0.76f);
        colors[ImGuiCol_HeaderHovered]         = ImVec4(0.45f, 0.20f, 0.30f, 0.86f);
        colors[ImGuiCol_HeaderActive]          = ImVec4(0.50f, 0.07f, 0.25f, 1.00f);
        colors[ImGuiCol_Separator]             = ImVec4(0.41f, 0.35f, 0.40f, 0.47f);
        colors[ImGuiCol_SeparatorHovered]      = ImVec4(0.10f, 0.40f, 0.75f, 0.78f);
        colors[ImGuiCol_SeparatorActive]       = ImVec4(0.11f, 0.52f, 1.00f, 1.00f);
        colors[ImGuiCol_ResizeGrip]            = ImVec4(0.47f, 0.77f, 0.83f, 0.04f);
        colors[ImGuiCol_ResizeGripHovered]     = ImVec4(0.45f, 0.20f, 0.30f, 0.78f);
        colors[ImGuiCol_ResizeGripActive]      = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_Tab]                   = ImVec4(0.45f, 0.20f, 0.30f, 0.76f);
        colors[ImGuiCol_TabHovered]            = ImVec4(0.45f, 0.20f, 0.30f, 0.86f);
        colors[ImGuiCol_TabActive]             = ImVec4(0.50f, 0.07f, 0.25f, 1.00f);
        colors[ImGuiCol_TabUnfocused]          = ImVec4(0.07f, 0.10f, 0.15f, 0.97f);
        colors[ImGuiCol_TabUnfocusedActive]    = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_DockingPreview]        = ImVec4(0.26f, 0.59f, 0.98f, 0.70f);
        colors[ImGuiCol_DockingEmptyBg]        = ImVec4(0.07f, 0.08f, 0.09f, 1.00f);
        colors[ImGuiCol_PlotLines]             = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        colors[ImGuiCol_PlotLinesHovered]      = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_PlotHistogram]         = ImVec4(0.86f, 0.93f, 0.89f, 0.63f);
        colors[ImGuiCol_PlotHistogramHovered]  = ImVec4(0.45f, 0.20f, 0.30f, 1.00f);
        colors[ImGuiCol_TableHeaderBg]         = ImVec4(0.19f, 0.19f, 0.20f, 1.00f);
        colors[ImGuiCol_TableBorderStrong]     = ImVec4(0.31f, 0.31f, 0.35f, 1.00f);
        colors[ImGuiCol_TableBorderLight]      = ImVec4(0.23f, 0.23f, 0.25f, 1.00f);
        colors[ImGuiCol_TableRowBg]            = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
        colors[ImGuiCol_TableRowBgAlt]         = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
        colors[ImGuiCol_TextSelectedBg]        = ImVec4(0.45f, 0.20f, 0.30f, 0.43f);
        colors[ImGuiCol_DragDropTarget]        = ImVec4(1.00f, 1.00f, 0.00f, 0.90f);
        colors[ImGuiCol_NavHighlight]          = ImVec4(0.26f, 0.59f, 0.98f, 1.00f);
        colors[ImGuiCol_NavWindowingHighlight] = ImVec4(1.00f, 1.00f, 1.00f, 0.70f);
        colors[ImGuiCol_NavWindowingDimBg]     = ImVec4(0.80f, 0.80f, 0.80f, 0.20f);
        colors[ImGuiCol_ModalWindowDimBg]      = ImVec4(0.80f, 0.80f, 0.80f, 0.35f);
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