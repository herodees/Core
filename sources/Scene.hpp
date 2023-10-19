#pragma once

#include "Include.hpp"

namespace box
{
	struct component_definition
	{
		std::string id;
		std::string name;
		entt::registry::base_type* storage;
	};



	class scene_impl final : public scene
	{
	public:
		scene_impl(game& gme);
		~scene_impl() override;

        void init();


        entity_id  create() override;
        void       release(entity_id id) override;
        bool       is_valid(entity_id id) const override;
        component* emplace(entity_id id, std::string_view component) override;
        void       remove(entity_id id, std::string_view component) override;
        bool       contains(entity_id id, std::string_view component) const override;
        bool       contains(entity_id id, const Storage** storage, size_t count) const override;
        void       patch(entity_id id, std::string_view component) override;
        void       add_tag(entity_id id, tag_id tag) override;
        void       remove_tag(entity_id id, tag_id tag) override;
        bool       contains_tag(entity_id id, tag_id tag) const override;
        bool       get_view(scene_view<1>* target, const tag_id* tags, size_t count) const override;
        bool       get_view(scene_view<1>* target, const std::string_view* components, size_t count) const override;
        system*    get_system(std::string_view sys) const override;
        game&      get_game() const override;

        const component_definition* find_component_definition(std::string_view id) const;
        void                        update(float delta_time);
        void                        on_frame_begin(float delta_time);
        void                        on_frame_end();
        entt::registry&             get_registry();

        template <typename C>
        const component_definition* register_component(std::string_view name, std::string_view id);
        template <typename S>
        const system* register_system(std::string_view name, std::string_view id);

    private:
        game&                                                                                       _game;
        entt::registry                                                                              _registry;
        std::vector<entt::sparse_set>                                                               _tags{};
        std::unordered_map<std::string, std::unique_ptr<system>, std::string_hash, std::equal_to<>> _systems{};
        std::unordered_map<std::string, component_definition, std::string_hash, std::equal_to<>>    _components{};
	};




	template<typename C>
	inline const component_definition* scene_impl::register_component(std::string_view name, std::string_view id)
    {
        auto it = _components.find(id);
        if (it != _components.end())
            return &it->second;
		C::definition = &_components
                                 .emplace(std::string(id),
                                          component_definition{std::string(id), std::string(name), &_registry.storage<C>()})
                                 .first->second;
		return C::definition;
    }

    template <typename S>
    inline const system* scene_impl::register_system(std::string_view name, std::string_view id)
    {
        auto it = _systems.find(id);
        if (it != _systems.end())
            return it->second.get();
        auto* sys = _systems.emplace(std::string(id), new S()).first->second.get();
        sys->init(*this);
        return sys;
    }


}