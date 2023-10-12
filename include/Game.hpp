#pragma once

#include "Asset.hpp"
#include "Graphics.hpp"
#include "Scene.hpp"

namespace box
{
    class game
    {
    public:
        virtual ~game() = default;

        virtual renderer&       get_renderer() = 0;
        virtual asset_provider& get_asset()    = 0;
        virtual plugin*         get_main()     = 0;
        virtual scene&          get_scene()    = 0;
    };

    class plugin
    {
    public:
        virtual ~plugin() = default;
    };
} // namespace box