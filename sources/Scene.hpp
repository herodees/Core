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

		ComponentInfo(std::string_view name, std::string_view id, entt::registry::base_type& storage)
			: _name(name), _id(id), _storage(storage){}
		[[nodiscard]] bool contains(entt::entity id) const noexcept { return _storage.contains(id); }
		void create(entt::entity id) const { _storage.emplace(id); }
		void remove(entt::entity id) const { _storage.remove(id); }
		void patch(entt::entity id) const { static_cast<entt::registry::storage_for_type<IComponent>&>(_storage).patch(id); }
		IComponent& get(entt::entity id) const { return *static_cast<IComponent*>(_storage.get(id)); }

		std::string _name;
		std::string _id;
		entt::registry::base_type& _storage;
	};



	class Scene : public IScene
	{
	public:
		Scene();
		~Scene() override;

		void init();

		EntityId create() override;
		void release(EntityId id) override;
		IComponent* add(EntityId id, std::string_view component) override;
		void remove(EntityId id, std::string_view component) override;
		bool contains(EntityId id, std::string_view component) const override;

		template <typename C>
		const ComponentInfo* register_component(std::string_view name, std::string_view id)
		{
			ComponentInfo info(name, id, _registry.storage<C>());
			const ComponentInfo& ret = *_components.insert(info).first;
			return &ret;
		}
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
}