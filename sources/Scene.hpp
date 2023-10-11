#pragma once

#include "Include.hpp"

namespace box
{
	struct ComponentDefinition
	{
		std::string id;
		std::string name;
		entt::registry::base_type* storage;

		static ComponentDefinition* create(std::string_view id, std::string_view name, entt::registry::base_type* str);
		static ComponentDefinition* find(std::string_view id);
	};



	class Scene : public IScene
	{
	public:
		Scene();
		~Scene() override;

		void init();

		EntityId create() override;
		void release(EntityId id) override;
		bool is_valid(EntityId id) const override;
		IComponent* emplace(EntityId id, std::string_view component) override;
		void remove(EntityId id, std::string_view component) override;
		bool contains(EntityId id, std::string_view component) const override;
		void patch(EntityId id, std::string_view component) override;
		void add_tag(EntityId id, TagId tag) override;
		void remove_tag(EntityId id, TagId tag) override;
		bool contains_tag(EntityId id, TagId tag) const override;
		bool get_view(View<1>* target, const TagId* tags, size_t count) const override;
		bool get_view(View<1>* target, const std::string_view* components, size_t count) const override;

		template <typename C>
		const ComponentDefinition* register_component(std::string_view name, std::string_view id);

		entt::registry& registry() { return _registry; }

	private:
		entt::registry _registry;
		std::vector<entt::sparse_set> _tags{};
	};



	template <typename T>
	struct Component : IComponent
	{
		static const ComponentDefinition* definition;

		static bool contains(EntityId id);
		static void patch(EntityId id);
		static void remove(EntityId id);
		static T* emplace(EntityId id);
	};



	template <typename T> const ComponentDefinition* Component<T>::definition = nullptr;

	template<typename C>
	inline const ComponentDefinition* Scene::register_component(std::string_view name, std::string_view id)
	{
		C::definition = ComponentDefinition::create(id, name, &_registry.storage<C>());
		return C::definition;
	}

	template<typename T>
	inline bool Component<T>::contains(EntityId id)
	{
		return Component<T>::definition->storage->contains((entt::entity)id);
	}

	template<typename T>
	inline void Component<T>::patch(EntityId id)
	{
		static_cast<entt::storage_for_t<T>*>(Component<T>::definition->storage)->patch((entt::entity)id);
	}

	template<typename T>
	inline void Component<T>::remove(EntityId id)
	{
		Component<T>::definition->storage->remove((entt::entity)id);
	}

	template<typename T>
	inline T* Component<T>::emplace(EntityId id)
	{
		return static_cast<T*>(Component<T>::definition->storage->get((entt::entity)id));
	}
}