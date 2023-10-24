#include "Tileset.hpp"

namespace box
{
    const component_definition* tileset_layer::definition = nullptr;

    tileset_impl::~tileset_impl()
    {
    }

    void tileset_impl::init(scene& scn)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);

        scene.register_component<tileset_layer>();
    }

    void tileset_impl::update(scene& scn, float delta)
    {
        scene_impl& scene = static_cast<scene_impl&>(scn);

        scene.get_registry().sort<tileset_layer>([](const auto& lhs, const auto& rhs) { return lhs._order < rhs._order; });
    }

} // namespace box


