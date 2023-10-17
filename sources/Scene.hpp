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
		scene_impl();
		~scene_impl() override;

		void init();

		entity_id create() override;
		void release(entity_id id) override;
		bool is_valid(entity_id id) const override;
		component* emplace(entity_id id, std::string_view component) override;
		void remove(entity_id id, std::string_view component) override;
		bool contains(entity_id id, std::string_view component) const override;
		bool contains(entity_id id, const Storage** storage, size_t count) const override;
		void patch(entity_id id, std::string_view component) override;
		void add_tag(entity_id id, tag_id tag) override;
		void remove_tag(entity_id id, tag_id tag) override;
		bool contains_tag(entity_id id, tag_id tag) const override;
		bool get_view(scene_view<1>* target, const tag_id* tags, size_t count) const override;
		bool get_view(scene_view<1>* target, const std::string_view* components, size_t count) const override;
        system* get_system(std::string_view sys) const override;

		template <typename C>
		const component_definition* register_component(std::string_view name, std::string_view id);

        template <typename S>
        const component_definition* register_system(std::string_view name, std::string_view id);

		const component_definition* find_component_definition(std::string_view id) const;

		entt::registry& registry() { return _registry; }

        void on_frame_begin(game& gme, float delta_time);
        void on_frame_end(game& gme);

	private:
		entt::registry _registry;
		std::vector<entt::sparse_set> _tags{};
        std::unordered_map<std::string, std::unique_ptr<system>, std::string_hash, std::equal_to<>> _systems{};
        std::unordered_map<std::string, component_definition, std::string_hash, std::equal_to<>> _components{};
	};



	template <typename T>
	struct component_for_t : component
	{
		static const component_definition* definition;

		static bool contains(entity_id id);
		static void patch(entity_id id);
		static void remove(entity_id id);
		static T* emplace(entity_id id);
	};



	template <typename T> const component_definition* component_for_t<T>::definition = nullptr;

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
    inline const component_definition* scene_impl::register_system(std::string_view name, std::string_view id)
    {
        auto it = _systems.find(id);
        if (it != _systems.end())
            return &it->second;
        return &_systems.emplace(std::string(id), new S()).first->second;
    }

	template<typename T>
	inline bool component_for_t<T>::contains(entity_id id)
	{
		return component_for_t<T>::definition->storage->contains((entt::entity)id);
	}

	template<typename T>
	inline void component_for_t<T>::patch(entity_id id)
	{
		static_cast<entt::storage_for_t<T>*>(component_for_t<T>::definition->storage)->patch((entt::entity)id);
	}

	template<typename T>
	inline void component_for_t<T>::remove(entity_id id)
	{
		component_for_t<T>::definition->storage->remove((entt::entity)id);
	}

	template<typename T>
	inline T* component_for_t<T>::emplace(entity_id id)
	{
		return static_cast<T*>(component_for_t<T>::definition->storage->get((entt::entity)id));
	}
}