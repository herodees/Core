#pragma once

#include "Asset.hpp"
#include "Graphics.hpp"
#include "Scene.hpp"
#include "Inputs.hpp"
#include "Video.hpp"

namespace box
{
    class game
    {
    public:
        virtual ~game() = default;

        virtual renderer&       get_renderer() = 0;
        virtual asset_provider& get_asset()    = 0;
        virtual plugin&         get_plugin()   = 0;
        virtual scene&          get_scene()    = 0;
        virtual inputs&         get_inputs()   = 0;
        virtual video&          get_video()    = 0;
        virtual physics&        get_physics()  = 0;
    };



    class plugin
    {
    public:
        virtual ~plugin() = default;

        virtual void on_init(game& gme){};
        virtual void on_deinit(game& gme){};

        virtual void on_frame_begin(game& gme, float delta_time){};
        virtual void on_frame_end(game& gme){};

        virtual void on_scene_begin(game& gme){};
        virtual void on_scene_end(game& gme){};

    };
} // namespace box