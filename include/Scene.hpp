#pragma once

namespace box
{
	class IScene;
	class Entity;

	using Storage = void;

	using EntityId = uint32_t;

	using TagId = uint8_t;

	struct IComponent {};


	struct StorageData
	{
		const EntityId* data;
		size_t size;
	};



	template <size_t SZE>
	class View
	{
	public:
		class Iterator
		{
		public:
			Iterator() = default;
			Iterator(const Iterator&) = default;
			Iterator(View* v, const EntityId* p) : _view(v), _ptr(p) { filter(); }
			Iterator& operator=(const Iterator&) = default;
			Iterator& operator++();
			Iterator operator++(int);
			EntityId operator*() const { return *_ptr; }
			bool operator==(const Iterator& it) const { return _ptr == it._ptr; }
			bool operator!=(const Iterator& it) const { return _ptr != it._ptr; }
			const EntityId* operator->() const { return *_ptr; }
			operator EntityId() const { return *_ptr; }
		private:
			void filter();
			View* _view{};
			const EntityId* _ptr{};
		};

		using iterator = Iterator;

		Iterator begin() { return Iterator(this, data); }
		Iterator end() { return Iterator(this, data + size); }
		Iterator begin() const { return Iterator(this, data); }
		Iterator end() const { return Iterator(this, data + size ); }

		IScene* _scene{};
		const EntityId* data{};
		size_t size{};
		const Storage* _storage[SZE];
	};



	class IBehavior
	{
	public:
		IBehavior() = default;
		virtual ~IBehavior() = default;

		virtual void on_create(Entity& self) {};
		virtual void on_destroy(Entity& self) {};
		virtual void on_step(Entity& self, float dt) {};
		virtual void on_static_step(Entity& self, float dt) {};
		virtual void on_render(Entity& self) {};
	};



	class IScene
	{
	public:
		virtual ~IScene() = default;

		virtual EntityId create() = 0;
		virtual void release(EntityId id) = 0;
		virtual bool is_valid(EntityId id) const = 0;
		virtual IComponent* emplace(EntityId id, std::string_view component) = 0;
		virtual void remove(EntityId id, std::string_view component) = 0;
		virtual bool contains(EntityId id, std::string_view component) const = 0;
		virtual bool contains(EntityId id, const Storage** storage, size_t count) const = 0;
		virtual void patch(EntityId id, std::string_view component) = 0;
		virtual void add_tag(EntityId id, TagId tag) = 0;
		virtual void remove_tag(EntityId id, TagId tag) = 0;
		virtual bool contains_tag(EntityId id, TagId tag) const = 0;
		virtual bool get_view(View<1>* target, const TagId* tags, size_t count) const = 0;
		virtual bool get_view(View<1>* target, const std::string_view* components, size_t count) const = 0;
	
		auto view(std::convertible_to<std::string_view> auto&& ...s) { return view<std::string_view, sizeof...(s)>(std::initializer_list<std::string_view>{ s... }); }
		auto view(std::convertible_to<TagId> auto&& ...s) { return view<TagId, sizeof...(s)>(std::initializer_list<TagId>{ s... }); }
		template <typename T, size_t S> auto view(std::initializer_list<T>&& list);

	};



	class Entity
	{
	public:
		IScene* _scene;
		EntityId _entity;
	};



	template<typename T, size_t S>
	inline auto IScene::view(std::initializer_list<T>&& list)
	{
		View<S> view;
		get_view((View<1>*)&view, list.begin(), list.size());
		return view;
	}


	template<size_t SZE>
	inline View<SZE>::Iterator& View<SZE>::Iterator::operator++()
	{
		++_ptr;
		filter();
		return *this;
	}

	template<size_t SZE>
	inline View<SZE>::Iterator View<SZE>::Iterator::operator++(int)
	{
		return Iterator(_view, _ptr + 1);
	}

	template<size_t SZE>
	inline void View<SZE>::Iterator::filter()
	{
		if (!_ptr) return;
		const auto end = _view->data + _view->size;

		while (!_view->_scene->contains(*_ptr, _view->_storage + 1, _view->size - 1) && _ptr != end)
		{
			++_ptr;
		}
	}
}