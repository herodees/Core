#pragma once

#include "Asset.hpp"
#include "Include.hpp"
#include "Plugin.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Device.hpp"
#include "Imgui.hpp"

namespace box
{
    class game_impl final : public game
    {
    public:
        game_impl();
        ~game_impl() override;

        void setup(const char* v[], int32_t c);

        int32_t         run();
        void            init();
        renderer&       get_renderer() override;
        asset_provider& get_asset() override;
        scene&          get_scene() override;
        physics&        get_physics() override;
        inputs&         get_inputs() override;
        video&          get_video() override;
        plugin&         get_plugin() override;
        imgui&          get_imgui() override;

    private:
        void on_frame_begin();
        void on_frame_end();
        void on_imgui();
        void on_update();

        static void em_arg_callback_func(void*);

    private:
        plugin*             _plugin;
        asset_provider_impl _assets;
        scene_impl          _scene;
        inputs_impl         _inputs;
        video_impl          _video;
        imgui_impl          _imgui;
        game_plugin         _game_plugin;
        const char**        _argv{};
        int32_t             _argc{};
    };
} // namespace box