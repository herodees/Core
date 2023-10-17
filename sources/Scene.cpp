#include "Scene.hpp"

namespace box
{


	scene_impl::scene_impl()
	{
	}

	scene_impl::~scene_impl()
	{
	}

	void scene_impl::init()
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

	entity_id scene_impl::create()
	{
		return (entity_id)_registry.create();
	}

	void scene_impl::release(entity_id id)
	{
		_registry.destroy((entt::entity)id);
	}

	bool scene_impl::is_valid(entity_id id) const
	{
		return _registry.valid((entt::entity)id);
	}

	component* scene_impl::emplace(entity_id id, std::string_view cmp_id)
	{
		if (auto* cmp = find_component_definition(cmp_id))
		{
			cmp->storage->emplace((entt::entity)id);
			return static_cast<component*>(cmp->storage->get((entt::entity)id));
		}
		return nullptr;
	}

	void scene_impl::remove(entity_id id, std::string_view cmp_id)
	{
        if (auto* cmp = find_component_definition(cmp_id))
		{
			cmp->storage->remove((entt::entity)id);
		}
	}

	bool scene_impl::contains(entity_id id, std::string_view cmp_id) const
	{
        if (auto* cmp = find_component_definition(cmp_id))
		{
			return cmp->storage->contains((entt::entity)id);
		}
		return false;
	}

	bool scene_impl::contains(entity_id id, const Storage** storage, size_t count) const
	{
		const entt::sparse_set** sets = reinterpret_cast<const entt::sparse_set**>(storage);
		for (size_t n = 0; n < count; ++n)
		{
			if (!sets[n]->contains((entt::entity)id))
				return false;
		}
		return true;
	}

	void scene_impl::patch(entity_id id, std::string_view cmp_id)
	{
        if (auto* cmp = find_component_definition(cmp_id))
		{
			static_cast<entt::storage_for_t<component>*>(cmp->storage)->patch((entt::entity)id);
		}
	}

	void scene_impl::add_tag(entity_id id, tag_id tag)
	{
		if (_tags.size() <= tag)
			_tags.resize(tag + 1);
		_tags[tag].emplace((entt::entity)id);
	}

	void scene_impl::remove_tag(entity_id id, tag_id tag)
	{
		if (_tags.size() <= tag)
			return;
		_tags[tag].remove((entt::entity)id);
	}

	bool scene_impl::contains_tag(entity_id id, tag_id tag) const
	{
		if (_tags.size() <= tag)
			return false;
		return _tags[tag].contains((entt::entity)id);
	}

	bool scene_impl::get_view(scene_view<1>* target, const tag_id* tags, size_t count) const
	{
		for (size_t n = 0; n < count; ++n)
		{
			if (_tags.size() <= tags[n])
			{
				target->_size = 0;
				return false;
			}
			target->_storage[n] = &_tags[n];
		}
		return true;
	}

	bool scene_impl::get_view(scene_view<1>* target, const std::string_view* components, size_t count) const
	{
		scene_view<0xff>& out = (scene_view<0xff>&)*target;
		out._scene = const_cast<scene_impl*>(this);
		for (size_t n = 0; n < count; ++n)
		{
            if (auto* cmp = find_component_definition(components[n]))
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
		out._size = a->size();
		out._data = (entity_id*)a->data();
		return true;
    }

    system* scene_impl::get_system(std::string_view sys) const
    {
        auto it = _systems.find(sys);
        if (it == _systems.end())
            return nullptr;
        return it->second.get();
    }

    const component_definition* scene_impl::find_component_definition(std::string_view id) const
    {
        auto it = _components.find(id);
        if (it == _components.end())
            return nullptr;
        return &it->second;
    }

    void scene_impl::on_frame_begin(game& gme, float delta_time)
    {
        for (auto& it : _systems)
        {
            it.second->on_frame_begin(gme, delta_time);
		}
    }

    void scene_impl::on_frame_end(game& gme)
    {
        for (auto& it : _systems)
        {
            it.second->on_frame_end(gme);
        }
    }

}