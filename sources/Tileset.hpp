#pragma once

#include "Include.hpp"
#include "Scene.hpp"

namespace box
{
    class tileset_impl final : public tileset
    {
    public:
        ~tileset_impl() override;

        void init(scene& scn) override;
        void update(scene& scn, float delta) override;
    };

    struct tileset_chunk
    {
        bool           _active{};
        bool           _initialized{};
        uint8_t*       _data{};
        int32_t        _size{};
        uint8_t*       _full_data{};
        int32_t        _full_size{};
        tileset_chunk* _next{};
    };

    struct tile_layer : tile_layer_component
    {
        static const component_definition* definition;

        void           set_active_region(Recti region) override;
        Recti          get_active_region() const override;
        void           set_tile(Vec2i pos, tile tle) override;
        tile           get_tile(Vec2i pos) override;
        uint8_t        get_atlas(atlas* atl) override;
        uint8_t        find_atlas(atlas* atl) const override;

        tileset_chunk* activate_chunk(Vec2i pos, bool activate);

        int32_t                        _order{};
        Vec2i                          _chunk_size{16,16};
        Recti                          _active_region;
        tileset_chunk*                 _active_chunks{};
        std::vector<asset_ref<atlas>>  _atlases;
        std::map<Vec2i, tileset_chunk> _chumks;
    };

    struct sprite_chunk
    {
        bool                _active{};
        std::vector<sprite> _sprites;
        sprite_chunk*       _next{};
    };

    struct sprite_layer : sprite_layer_component
    {
        static const component_definition* definition;

        int32_t                       _order{};
        sprite_chunk*                 _active_chunks{};
        Vec2i                         _chunk_size{128, 128};
        std::vector<asset_ref<atlas>> _atlases;
        std::map<Vec2i, sprite_chunk> _chumks;
    };
} // namespace box