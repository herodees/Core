#include "Scene.hpp"

namespace box
{
	static std::unordered_map<std::string, ComponentDefinition, std::string_hash, std::equal_to<>> s_somponents {};

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
		entt::storage_for_t<std::remove_const_t<void>> storage;
		entt::sparse_set set;

		set.emplace({});
		bool val = set.contains({});
		set.remove({});
		val = set.contains({});

		if (val)
			val;
	}

	EntityId Scene::create()
	{
		return (EntityId)_registry.create();
	}

	void Scene::release(EntityId id)
	{
		_registry.destroy((entt::entity)id);
	}

	bool Scene::is_valid(EntityId id) const
	{
		return _registry.valid((entt::entity)id);
	}

	component* Scene::emplace(EntityId id, std::string_view cmp_id)
	{
		if (auto* cmp = ComponentDefinition::find(cmp_id))
		{
			cmp->storage->emplace((entt::entity)id);
			return static_cast<component*>(cmp->storage->get((entt::entity)id));
		}
		return nullptr;
	}

	void Scene::remove(EntityId id, std::string_view cmp_id)
	{
		if (auto* cmp = ComponentDefinition::find(cmp_id))
		{
			cmp->storage->remove((entt::entity)id);
		}
	}

	bool Scene::contains(EntityId id, std::string_view cmp_id) const
	{
		if (auto* cmp = ComponentDefinition::find(cmp_id))
		{
			return cmp->storage->contains((entt::entity)id);
		}
		return false;
	}

<<<<<<< HEAD
	bool Scene::contains(EntityId id, const Storage** storage, size_t count) const
	{
		const entt::sparse_set** sets = reinterpret_cast<const entt::sparse_set**>(storage);
		for (size_t n = 0; n < count; ++n)
		{
			if (!sets[n]->contains((entt::entity)id))
				return false;
		}
		return true;
	}

	void Scene::patch(EntityId id, std::string_view component)
=======
	void Scene::patch(EntityId id, std::string_view cmp_id)
>>>>>>> 50a6a12781b478d07bc57849f67735a89f083beb
	{
		if (auto* cmp = ComponentDefinition::find(cmp_id))
		{
			static_cast<entt::storage_for_t<component>*>(cmp->storage)->patch((entt::entity)id);
		}
	}

	void Scene::add_tag(EntityId id, TagId tag)
	{
		if (_tags.size() <= tag)
			_tags.resize(tag + 1);
		_tags[tag].emplace((entt::entity)id);
	}

	void Scene::remove_tag(EntityId id, TagId tag)
	{
		if (_tags.size() <= tag)
			return;
		_tags[tag].remove((entt::entity)id);
	}

	bool Scene::contains_tag(EntityId id, TagId tag) const
	{
		if (_tags.size() <= tag)
			return false;
		return _tags[tag].contains((entt::entity)id);
	}

	bool Scene::get_view(View<1>* target, const TagId* tags, size_t count) const
	{
		for (size_t n = 0; n < count; ++n)
		{
			if (_tags.size() <= tags[n])
			{
				target->size = 0;
				return false;
			}
			target->_storage[n] = &_tags[n];
		}


		return true;
	}

	bool Scene::get_view(View<1>* target, const std::string_view* components, size_t count) const
	{
		View<0xff>& out = (View<0xff>&)*target;
		out._scene = const_cast<Scene*>(this);
		for (size_t n = 0; n < count; ++n)
		{
			if (auto* cmp = ComponentDefinition::find(components[n]))
			{
				out._storage[n] = cmp->storage;
			}
			else
			{
				return false;
			}
		}
		int min_idx;
		for (int i = 0; i < int(count) - 1; i++)
		{
			min_idx = i;
			for (int j = i + 1; j < int(count); j++)
			{
				const entt::sparse_set* a = (entt::sparse_set*)out._storage[j];
				const entt::sparse_set* b = (entt::sparse_set*)out._storage[min_idx];
				if (a->size() < b->size())
					min_idx = j;
			}
			if (min_idx != i)
				std::swap(target->_storage[min_idx], target->_storage[i]);
		}

		const entt::sparse_set* a = (entt::sparse_set*)out._storage[0];
		out.size = a->size();
		out.data = (EntityId*)a->data();
		return true;
	}

}