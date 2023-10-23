#pragma once

namespace box
{
    struct vertex;
    struct component;
    struct camera;
    struct color;
    struct rigid_body_component;
    struct collider_component;
    struct collider_filter;
    struct circle_collider_component;
    struct segment_collider_component;
    struct polygon_collider_component;
    class asset_provider;
    class texture;
    class material;
    class prototype;
    class sound;
    class plugin;
    class scene;
    class renderer;
    class entity;
    class system;
    class game;
    class atlas;
    class physics;
    class tileset;
    class render_texture;
    enum class blend_mode;
    enum class uniform_type;
} // namespace box

#include <atomic>
#include <charconv>
#include <string_view>
#include <cmath>
#include <cstdint>

#include "Var.hpp"
#include "Vec.hpp"
#include "Rect.hpp"
#include "Asset.hpp"
#include "Components.hpp"
#include "Export.hpp"
#include "Game.hpp"
#include "Scene.hpp"
#include "Graphics.hpp"
#include "Physics.hpp"
#include "Tileset.hpp"
