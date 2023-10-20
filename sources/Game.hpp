#pragma once

#include "Asset.hpp"
#include "Include.hpp"
#include "Plugin.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Device.hpp"

namespace box
{
    class game_impl final : public game
    {
    public:
        game_impl();
        ~game_impl() override;

        int32_t         run(const char* v[], int32_t c);
        void            init(const char* v[], int32_t c);
        renderer&       get_renderer() override;
        asset_provider& get_asset() override;
        scene&          get_scene() override;
        physics&        get_physics() override;
        inputs&         get_inputs() override;
        video&          get_video() override;
        plugin&         get_plugin() override;

    private:
        void on_frame_begin();
        void on_frame_end();

    private:
        plugin*             _plugin;
        asset_provider_impl _assets;
        scene_impl          _scene;
        inputs_impl         _inputs;
        video_impl          _video;
        game_plugin         _game_plugin;
    };
} // namespace box