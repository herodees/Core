#include "Scene.hpp"

namespace box
{
	Scene::Scene()
	{
	}

	Scene::~Scene()
	{
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

		return it->_create(_registry, (entt::entity)id);
	}

	void Scene::remove(EntityId id, std::string_view component)
	{
		auto it = _components.find(component);
		if (it == _components.end())
			return;

		return it->_remove(_registry, (entt::entity)id);
	}

	bool Scene::contains(EntityId id, std::string_view component) const
	{
		auto it = _components.find(component);
		if (it == _components.end())
			return false;

		return it->_contains(_registry, (entt::entity)id);
	}
}