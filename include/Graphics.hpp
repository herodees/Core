#pragma once

#include "Vec.hpp"
#include "Rect.hpp"
#include "Scene.hpp"
#include <span>

namespace box
{
    enum class blend_mode
    {
        ALPHA = 0,
        ADDITIVE,
        MULTIPLIED,
        ADD_COLORS,
        SUBTRACT_COLORS,
        ALPHA_PREMULTIPLY,
        CUSTOM,
        CUSTOM_SEPARATE
    };

    enum class uniform_type
    {
        FLOAT = 0,
        VEC2,
        VEC3,
        VEC4,
        INT,
        IVEC2,
        IVEC3,
        IVEC4,
        SAMPLER2D
    };

    struct color
    {
        uint8_t r;
        uint8_t g;
        uint8_t b;
        uint8_t a;
    };

    struct camera
    {
        Vec2f offset;
        Vec2f target;
        float rotation{0.f};
        float zoom{1.f};
    };

    struct vertex
    {
        Vec2f position;
        color color;
        Vec2f tex_coord;
    };

    struct mesh
    {
        vertex*  vertex{};
        uint32_t vertex_size{};
    };

    class render_texture
    {
    public:
        virtual ~render_texture()                                                          = default;
        virtual const texture& get_texture() const                                         = 0;
        virtual const texture& get_depth() const                                           = 0;
        virtual bool           create(uint32_t width, uint32_t height, bool depth = false) = 0;
        const Vec2i&           size() const
        {
            return _size;
        }

    protected:
        uint32_t _id{};
        Vec2i    _size;
    };

    class renderer : system
    {
    public:
        virtual ~renderer()                                                                              = default;
        virtual void            clear_background(color c)                                                = 0;
        virtual bool            begin_2d(const camera& cam, bool depthsort)                              = 0;
        virtual void            end_2d()                                                                 = 0;
        virtual void            enable_scissor_test(const Recti& scissor)                                = 0;
        virtual void            enable_render_texture(const render_texture* rt)                          = 0;
        virtual void            set_texture(const texture* texture)                                      = 0;
        virtual void            set_depth(uint32_t depth)                                                = 0;
        virtual void            set_material(material* material)                                         = 0;
        virtual material*       get_default_material()                                                   = 0;
        virtual material*       get_material()                                                           = 0;
        virtual mesh            begin_mesh(uint32_t vertex)                                              = 0;
        virtual void            end_mesh(const mesh& mesh)                                               = 0;
        virtual render_texture* load_render_texture(uint32_t width, uint32_t height, bool depth = false) = 0;

        virtual void            draw_line(Vec2f p1, Vec2f p2, color clr)                                 = 0;
        virtual void            draw_polyline(const Vec2f* p, size_t size, bool closed, color clr)       = 0;
        virtual void            draw_circle_segment(Vec2f    center,
                                                    float    radius,
                                                    color    clr,
                                                    uint32_t segments,
                                                    float    start_ang = 0.f,
                                                    float    end_ang   = 360.f)                          = 0;
        virtual void            draw_rectangle(Rectf rc, color clr)                                      = 0;
        virtual void            draw_ellipse(Rectf rc, color clr)                                        = 0;
    };

    struct mesh_renderer : component
    {
        void set_mesh(const vertex* vtx, size_t size);

        const vertex*       _corners;
        size_t              _size;
        uint32_t            _depth;
        asset_ref<texture>  _texture;
        asset_ref<material> _material;
    };



    inline void mesh_renderer::set_mesh(const vertex* vtx, size_t size)
    {
        _corners = vtx;
        _size    = size;
    }
} // namespace box