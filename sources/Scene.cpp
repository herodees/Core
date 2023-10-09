#include "Scene.hpp"

namespace box
{
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

	IComponent* Scene::add(EntityId id, std::string_view component)
	{
		auto it = _components.find(component);
		if(it == _components.end())
			return nullptr;
		it->create((entt::entity)id);
		return &it->get((entt::entity)id);
	}

	void Scene::remove(EntityId id, std::string_view component)
	{
		auto it = _components.find(component);
		if (it == _components.end())
			return;
		return it->remove((entt::entity)id);
	}

	bool Scene::contains(EntityId id, std::string_view component) const
	{
		auto it = _components.find(component);
		if (it == _components.end())
			return false;
		return it->contains((entt::entity)id);
	}
}