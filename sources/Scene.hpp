#pragma once

#include "Include.hpp"

namespace box
{
	struct ComponentInfo
	{
		struct CompareId
		{
			using is_transparent = void;
			bool operator()(ComponentInfo const& a, ComponentInfo const& b) const
			{
				return a._id < b._id;
			}
			bool operator()(std::string_view const& id, ComponentInfo const& b) const
			{
				return id < b._id;
			}
			bool operator()(ComponentInfo const& a, std::string_view const& id) const
			{
				return a._id < id;
			}
		};

		std::string _name;
		std::string _id;
		IComponent* (*_create)(entt::registry& reg, entt::entity id);
		void (*_remove)(entt::registry& reg, entt::entity id);
		bool (*_contains)(const entt::registry& reg, entt::entity id);
	};



	class Scene : public IScene
	{
	public:
		Scene();
		~Scene() override;

		EntityId create() override;
		void release(EntityId id) override;
		IComponent* add(EntityId id, std::string_view component) override;
		void remove(EntityId id, std::string_view component) override;
		bool contains(EntityId id, std::string_view component) const override;

		template <typename C>
		const ComponentInfo* register_component(std::string_view name, std::string_view id);

	private:
		entt::registry _registry;
		std::set<ComponentInfo, ComponentInfo::CompareId> _components;
	};



	template <typename T>
	struct Component : IComponent
	{
		static const ComponentInfo* info;
	};

	template <typename T> const ComponentInfo* Component<T>::info = nullptr;

	template<typename C>
	inline const ComponentInfo* Scene::register_component(std::string_view name, std::string_view id)
	{
		ComponentInfo info;
		info._name = name;
		info._id = id;
		if constexpr(std::is_empty_v<C>)
			info._create = [](entt::registry& reg, entt::entity id) -> IComponent* { reg.emplace_or_replace<C>(id); return nullptr; };
		else
			info._create = [](entt::registry& reg, entt::entity id) -> IComponent* { return &reg.emplace_or_replace<C>(id); };
		info._remove = [](entt::registry& reg, entt::entity id) -> void { reg.remove<C>(id); };
		info._contains = [](const entt::registry& reg, entt::entity id) -> bool { return reg.any_of<C>(id); };
		const ComponentInfo& ret = *_components.insert(info).first;
		return &ret;
	}
}