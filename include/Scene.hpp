#pragma once

namespace box
{
    using Storage = void;

    using entity_id = uint32_t;

    using tag_id = uint8_t;

    struct component
    {
    };


    template <size_t SZE>
    class scene_view
    {
    public:
        class iterator
        {
        public:
            iterator()                = default;
            iterator(const iterator&) = default;
            iterator(scene_view* v, const entity_id* p) : _view(v), _ptr(p)
            {
                filter();
            }
            iterator& operator=(const iterator&) = default;
            iterator& operator++();
            iterator  operator++(int);
            entity_id operator*() const
            {
                return *_ptr;
            }
            bool operator==(const iterator& it) const
            {
                return _ptr == it._ptr;
            }
            bool operator!=(const iterator& it) const
            {
                return _ptr != it._ptr;
            }
            const entity_id* operator->() const
            {
                return *_ptr;
            }
            operator entity_id() const
            {
                return *_ptr;
            }

        private:
            void             filter();
            scene_view*      _view{};
            const entity_id* _ptr{};
        };

        iterator begin()
        {
            return iterator(this, _data);
        }
        iterator end()
        {
            return iterator(this, _data + _size);
        }
        iterator begin() const
        {
            return iterator(this, _data);
        }
        iterator end() const
        {
            return iterator(this, _data + _size);
        }

        template <size_t N>
        scene_view<N + SZE> combine(const scene_view<N>& view) const;

        bool contains(entity_id id) const;

        scene*           _scene{};
        const entity_id* _data{};
        size_t           _size{};
        const Storage*   _storage[SZE];
    };



    class behavior
    {
    public:
        behavior()          = default;
        virtual ~behavior() = default;

        virtual void on_create(entity& self){};
        virtual void on_destroy(entity& self){};
        virtual void on_step(entity& self, float dt){};
        virtual void on_static_step(entity& self, float dt){};
        virtual void on_render(entity& self){};
    };



    class scene
    {
    public:
        virtual ~scene() = default;

        virtual entity_id  create()                                                                                = 0;
        virtual void       release(entity_id id)                                                                   = 0;
        virtual bool       is_valid(entity_id id) const                                                            = 0;
        virtual component* emplace(entity_id id, std::string_view component)                                       = 0;
        virtual void       remove(entity_id id, std::string_view component)                                        = 0;
        virtual bool       contains(entity_id id, std::string_view component) const                                = 0;
        virtual bool       contains(entity_id id, const Storage** storage, size_t count) const                     = 0;
        virtual void       patch(entity_id id, std::string_view component)                                         = 0;
        virtual void       add_tag(entity_id id, tag_id tag)                                                       = 0;
        virtual void       remove_tag(entity_id id, tag_id tag)                                                    = 0;
        virtual bool       contains_tag(entity_id id, tag_id tag) const                                            = 0;
        virtual bool       get_view(scene_view<1>* target, const tag_id* tags, size_t count) const                 = 0;
        virtual bool       get_view(scene_view<1>* target, const std::string_view* components, size_t count) const = 0;
        
        auto view(std::convertible_to<std::string_view> auto&&... s);
        auto view(std::convertible_to<tag_id> auto&&... s);
        template <typename T, size_t S>
        auto view(std::initializer_list<T>&& list);
    };



    class entity
    {
    public:
        scene*    _scene;
        entity_id _entity;
    };



    template <typename T, size_t S>
    inline auto scene::view(std::initializer_list<T>&& list)
    {
        scene_view<S> view;
        get_view((scene_view<1>*)&view, list.begin(), list.size());
        return view;
    }

    inline auto scene::view(std::convertible_to<std::string_view> auto&&... s)
    {
        return view<std::string_view, sizeof...(s)>(std::initializer_list<std::string_view>{s...});
    }

    inline auto scene::view(std::convertible_to<tag_id> auto&&... s)
    {
        return view<tag_id, sizeof...(s)>(std::initializer_list<tag_id>{s...});
    }

    template <size_t SZE>
    inline scene_view<SZE>::iterator& scene_view<SZE>::iterator::operator++()
    {
        ++_ptr;
        filter();
        return *this;
    }

    template <size_t SZE>
    inline scene_view<SZE>::iterator scene_view<SZE>::iterator::operator++(int)
    {
        return iterator(_view, _ptr + 1);
    }

    template <size_t SZE>
    inline void scene_view<SZE>::iterator::filter()
    {
        if (!_ptr)
            return;
        const auto end = _view->_data + _view->_size;

        while (!_view->_scene->contains(*_ptr, _view->_storage + 1, _view->_size - 1) && _ptr != end)
        {
            ++_ptr;
        }
    }

    template <size_t SZE>
    inline bool scene_view<SZE>::contains(entity_id id) const
    {
        return _scene->_scene->contains(id, _storage, _size);
    }

    template <size_t SZE>
    template <size_t N>
    inline scene_view<N + SZE> scene_view<SZE>::combine(const scene_view<N>& view) const
    {
        scene_view<N + SZE> out;
        out._scene = _scene;
        if constexpr (N < SZE)
        {
            out._data = view._data;
            out._size = view._size;
            for (size_t n = 0; n < N; ++n)
            {
                out._storage[n] = view._storage[n];
            }
            for (size_t n = 0; n < SZE; ++n)
            {
                out._storage[N + n] = _storage[n];
            }
        }
        else
        {
            out._data = _data;
            out._size = _size;
            for (size_t n = 0; n < SZE; ++n)
            {
                out._storage[n] = view._storage[n];
            }
            for (size_t n = 0; n < N; ++n)
            {
                out._storage[SZE + n] = view._storage[n];
            }
        }
        return out;
    }

} // namespace box