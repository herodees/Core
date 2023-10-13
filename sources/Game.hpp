#pragma once

#include "Asset.hpp"
#include "Include.hpp"
#include "Plugin.hpp"
#include "Renderer.hpp"
#include "Scene.hpp"
#include "Device.hpp"

namespace box
{
    class game_impl : public game
    {
    public:
        game_impl();
        ~game_impl() override;

        int32_t         run(const char* v[], int32_t c);
        void            init(const char* v[], int32_t c);
        renderer&       get_renderer() override final;
        asset_provider& get_asset() override final;
        scene&          get_scene() override final;
        inputs&         get_inputs() override final;
        video&          get_video() override final;
        plugin&         get_main() override final;

    private:
        plugin*             _plugin;
        renderer_impl       _renderer;
        asset_provider_impl _assets;
        scene_impl          _scene;
        inputs_impl         _inputs;
        video_impl          _video;
        game_plugin         _game_plugin;
    };
} // namespace box