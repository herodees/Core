#pragma once

#include "Include.hpp"

namespace box
{
	struct component_definition
	{
		std::string id;
		std::string name;
		entt::registry::base_type* storage;

		static component_definition* create(std::string_view id, std::string_view name, entt::registry::base_type* str);
		static component_definition* find(std::string_view id);
	};



	class scene_impl : public scene
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

		template <typename C>
		const component_definition* register_component(std::string_view name, std::string_view id);

		entt::registry& registry() { return _registry; }

	private:
		entt::registry _registry;
		std::vector<entt::sparse_set> _tags{};
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
		C::definition = component_definition::create(id, name, &_registry.storage<C>());
		return C::definition;
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