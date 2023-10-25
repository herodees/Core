#pragma once

namespace box
{
    struct tile
    {
        uint8_t  _atlas;
        uint8_t  _flags;
        uint16_t _tile;
    };

    class tileset : public system
    {
    public:
        static constexpr component_type_info type_info = {"tileset", "Tileset"};

        virtual ~tileset() = default;
    };

    struct tile_layer_component : component
    {
        static constexpr component_type_info type_info = {"tile_layer", "Tile layer"};

        virtual ~tile_layer_component()                 = default;
        virtual void    set_active_region(Recti region) = 0;
        virtual Recti   get_active_region() const       = 0;
        virtual void    set_tile(Vec2i pos, tile tle)   = 0;
        virtual tile    get_tile(Vec2i pos)             = 0;
        virtual uint8_t get_atlas(atlas* atl)           = 0;
        virtual uint8_t find_atlas(atlas* atl) const    = 0;
    };

    struct sprite_layer_component : component
    {
        static constexpr component_type_info type_info = {"sprite_layer", "Sprite layer"};

        virtual ~sprite_layer_component() = default;
    };
} // namespace box