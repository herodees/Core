#pragma once

#include "Scene.hpp"
#include "Graphics.hpp"

namespace box
{
    struct component
    {
    };

    struct c_transform : component
    {
        Vec2f position{};
        Vec2f scale{1.f, 1.f};
        float rotation{0};
    };

    struct c_rigidbody : component
    {
        float mass{};
        float drag{};
        float angular_drag{};
        bool  gravity{};
    };

    struct c_constant_force : component
    {
        Vec2f force;
        float torque{};
    };

    struct c_camera : component
    {
        uint32_t        clear;
        color           bg_color{0, 0, 0, 255};
        bool            msaa{};
        Rectf           viewport{0, 0, 1.f, 1.f};
        render_texture* target{};
    };

    struct c_colider : component
    {
        bool  is_trigger{};
        Rectf region;
        Vec2f center;
    };

    struct c_sprite_renderer : component
    {
        uint32_t  sprite{};
        color     color{255, 255, 255, 255};
        material* material{};
        bool      flip_x{};
        bool      flip_y{};
        uint32_t  sorting_layer{};
        uint32_t  order{};
    };

    struct c_animator : component
    {
        uint32_t animation{};
        uint32_t animations{};
        bool     play;
    };
} // namespace box