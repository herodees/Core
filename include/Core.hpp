#pragma once

namespace box
{
    struct vertex;
    struct component;
    class asset_provider;
    class texture;
    class material;
    class prototype;
    class sound;
    class plugin;
    class renderer;
    class scene;
    class entity;
    class system;
    class game;
    enum class blend_mode;
    enum class uniform_type;
} // namespace box

#include <atomic>
#include <charconv>
#include <string_view>
#include <cmath>
#include <cstdint>

#include "Asset.hpp"
#include "Components.hpp"
#include "Export.hpp"
#include "Game.hpp"
#include "Graphics.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include "Var.hpp"
#include "Vec.hpp"


