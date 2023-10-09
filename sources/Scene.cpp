#include "Scene.hpp"

namespace box
{
	struct string_hash
	{
		using is_transparent = void;
		[[nodiscard]] size_t operator()(const char* txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(std::string_view txt) const {
			return std::hash<std::string_view>{}(txt);
		}
		[[nodiscard]] size_t operator()(const std::string& txt) const {
			return std::hash<std::string>{}(txt);
		}
	};

	static std::unordered_map<std::string, ComponentDefinition, string_hash, std::equal_to<>> s_somponents {};

	ComponentDefinition* ComponentDefinition::create(std::string_view id, std::string_view name, entt::registry::base_type* str)
	{
		auto it = s_somponents.find(id);
		if(it != s_somponents.end())
			return &it->second;
		return &s_somponents.emplace(std::string(id), ComponentDefinition{ std::string(id), std::string(name), str }).first->second;
	}

	ComponentDefinition* ComponentDefinition::find(std::string_view id)
	{
		auto it = s_somponents.find(id);
		if (it == s_somponents.end())
			return nullptr;
		return &it->second;
	}

	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
	}

	void Scene::init()
	{
		auto* cmp = _registry.storage(0);
		cmp = cmp;
	}

	EntityId Scene::create()
	{
		return (EntityId)_registry.create();
	}

	void Scene::release(EntityId id)
	{
		_registry.destroy((entt::entity)id);
	}

	IComponent* Scene::emplace(EntityId id, std::string_view component)
	{
		if (auto* cmp = ComponentDefinition::find(component))
		{
			cmp->storage->emplace((entt::entity)id);
			return static_cast<IComponent*>(cmp->storage->get((entt::entity)id));
		}
		return nullptr;
	}

	void Scene::remove(EntityId id, std::string_view component)
	{
		if (auto* cmp = ComponentDefinition::find(component))
		{
			cmp->storage->remove((entt::entity)id);
		}
	}

	bool Scene::contains(EntityId id, std::string_view component) const
	{
		if (auto* cmp = ComponentDefinition::find(component))
		{
			return cmp->storage->contains((entt::entity)id);;
		}
		return false;
	}

	void Scene::patch(EntityId id, std::string_view component)
	{
		if (auto* cmp = ComponentDefinition::find(component))
		{
			static_cast<entt::storage_for_t<IComponent>*>(cmp->storage)->patch((entt::entity)id);
		}
	}


}