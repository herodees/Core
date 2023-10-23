#pragma once

namespace box
{
    class tileset : public system
    {
    public:
        static constexpr component_type_info type_info = {"tileset", "Tileset"};

         virtual ~tileset() = default;
    };

    struct tileset_layer_component : component
    {
        static constexpr component_type_info type_info = {"tileset_layer", "Tileset layer"};

        virtual ~tileset_layer_component() = default;
    };

} // namespace box