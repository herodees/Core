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

    struct tileset_layer : tileset_layer_component
    {
        static const component_definition* definition;

        int32_t _order{};
    };

} // namespace box