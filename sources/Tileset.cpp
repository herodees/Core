#include "Tileset.hpp"

namespace box
{
    const component_definition* tile_layer::definition = nullptr;
    const component_definition* sprite_layer::definition = nullptr;

    tileset_impl::~tileset_impl()
    {
    }

    void tileset_impl::init(scene& scn)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);

        scene.register_component<tile_layer>();
        scene.register_component<sprite_layer>();
    }

    void tileset_impl::update(scene& scn, float delta)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);

        scene.get_registry().sort<tile_layer>([](const auto& lhs, const auto& rhs) { return lhs._order < rhs._order; });
    }

    void tile_layer::set_active_region(Recti region)
    {
        if (_active_region == region)
            return;

        _active_region = region;

        Recti reg;
        reg.min = _active_region.min / _chunk_size;
        reg.max = _active_region.max / _chunk_size;

        if (reg.min.x == reg.max.x)
            reg.max.x += 1;
        if (reg.min.y == reg.max.y)
            reg.max.y += 1;

        for (int32_t x = reg.min.x; x < reg.max.x; ++x)
        {
            for (int32_t y = reg.min.y; y < reg.max.y; ++y)
            {
                activate_chunk({x, y}, true);
            }
        }

        auto* chunk = _active_chunks;
        while (chunk)
        {
            chunk = chunk->_next;
        }
    }

    Recti tile_layer::get_active_region() const
    {
        return _active_region;
    }

    void tile_layer::set_tile(Vec2i pos, tile tle)
    {
        const Vec2i chunkpos = pos / _chunk_size;
        auto*       chunk    = activate_chunk(chunkpos, true);
        const Vec2i loc{pos.x - chunkpos.x * _chunk_size.x, pos.y - chunkpos.y * _chunk_size.y};
        tile* tiles = (tile*)chunk->_full_data;
        tiles[loc.x + loc.y * _chunk_size.x] = tle;
    }

    tile tile_layer::get_tile(Vec2i pos)
    {
        const Vec2i chunkpos = pos / _chunk_size;
        auto*       chunk    = activate_chunk(chunkpos, true);
        const Vec2i loc{pos.x - chunkpos.x * _chunk_size.x, pos.y - chunkpos.y * _chunk_size.y};
        tile*       tiles                    = (tile*)chunk->_full_data;
        return tiles[loc.x + loc.y * _chunk_size.x];
    }

    uint8_t tile_layer::get_atlas(atlas* atl)
    {
        if (auto aid = find_atlas(atl))
        {
            return aid;
        }
        _atlases.emplace_back(atl);
        return (uint8_t)_atlases.size();
    }

    uint8_t tile_layer::find_atlas(atlas* atl) const
    {
        if (atl)
        {
            uint8_t idx{};
            for (auto& a : _atlases)
            {
                ++idx;
                if (a.get() == atl)
                    return idx;
            }
        }
        return 0;
    }

    tileset_chunk* tile_layer::activate_chunk(Vec2i pos, bool activate)
    {
        tileset_chunk& chunk = _chumks[pos];

        if (!chunk._initialized)
        {
            chunk._full_size = _chunk_size.x * _chunk_size.y * sizeof(tile);
            chunk._full_data = (uint8_t*)ray::MemAlloc(chunk._full_size);
            std::memset(chunk._full_data, 0, chunk._full_size);
            chunk._active    = true;
            chunk._initialized = true;
        }

        if (chunk._active == activate)
            return &chunk;

        if (activate)
        {
            chunk._full_data = ray::DecompressData(chunk._data, chunk._size, &chunk._full_size);
            ray::MemFree(chunk._data);
            chunk._data = nullptr;
            chunk._size = 0;
            chunk._next    = _active_chunks;
            _active_chunks = &chunk;
        }
        else
        {
            chunk._data = ray::CompressData(chunk._full_data, chunk._full_size, &chunk._size);
            ray::MemFree(chunk._full_data);
            chunk._full_data = nullptr;
            chunk._full_size = 0;
        }

        chunk._active = activate;
        return &chunk;
    }

} // namespace box
