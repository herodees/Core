#pragma once

namespace box
{
    using Storage = void;

    using entity_id = uint32_t;

    using tag_id = uint8_t;



    class entity
    {
    public:
        entity() = default;
        entity(scene* scn, entity_id id);
        entity(const entity& ent);
        entity& operator=(const entity& ent);
        explicit operator bool() const;

        bool      is_valid() const;

        entity_id get_id() const;
        scene*    get_scene() const;

        behavior*  get_behavior(std::string_view id) const;
        component* get_component(std::string_view id) const;
        void       patch_component(std::string_view id);

        bool contains_behavior(std::string_view id) const;
        bool contains_component(std::string_view id) const;
        bool contains_tag(tag_id id) const;

        behavior*  add_behavior(std::string_view id);
        component* add_component(std::string_view id);
        void       add_tag(tag_id id);

        void remove_behavior(std::string_view id);
        void remove_component(std::string_view id);
        void remove_tag(tag_id id);

    private:
        scene* _scene{};
        entity_id _id{};
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
            const entity* operator->() const
            {
                return &_ent;
            }
            operator entity&() const
            {
                return _ent;
            }

        private:
            void             filter();
            scene_view*      _view{};
            const entity_id* _ptr{};
            entity           _ent{};
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
        struct factory;

    public:
        behavior()          = default;
        virtual ~behavior() = default;

        virtual void on_step(float dt){};
        virtual void on_static_step(float dt){};
        virtual void on_render(){};

        template <typename T>
        T* as()
        {
            return static_cast<T*>(this);
        }

        template <typename T>
        const T* as() const
        {
            return static_cast<const T*>(this);
        }
    };



    class scene
    {
    public:
        scene()          = default;
        virtual ~scene() = default;

        template <typename B>
        scene& register_behavior(std::string_view id, std::string_view name);

        virtual game& get_game() const = 0;

        virtual entity_id  create()                                                                                = 0;
        virtual void       release(entity_id id)                                                                   = 0;
        virtual bool       is_valid(entity_id id) const                                                            = 0;
        virtual component* add_component(entity_id id, std::string_view component)                                 = 0;
        virtual component* get_component(entity_id id, std::string_view component)                                 = 0;
        virtual void       remove_component(entity_id id, std::string_view component)                              = 0;
        virtual bool       contains_component(entity_id id, std::string_view component) const                      = 0;
        virtual void       patch_component(entity_id id, std::string_view component)                               = 0;
        virtual void       add_tag(entity_id id, tag_id tag)                                                       = 0;
        virtual void       remove_tag(entity_id id, tag_id tag)                                                    = 0;
        virtual bool       contains_tag(entity_id id, tag_id tag) const                                            = 0;
        virtual void       add_behavior(entity_id id, std::string_view beh_id)                                     = 0;
        virtual void       remove_behavior(entity_id id, std::string_view beh_id)                                  = 0;
        virtual bool       contains_behavior(entity_id id, std::string_view beh_id) const                          = 0;
        virtual behavior*  get_behavior(entity_id id, std::string_view beh_id)                                     = 0;
        virtual bool       get_view(scene_view<1>* target, const tag_id* tags, size_t count) const                 = 0;
        virtual bool       get_view(scene_view<1>* target, const std::string_view* components, size_t count) const = 0;
        virtual system*    get_system(std::string_view sys) const                                                  = 0;

        virtual behavior::factory* register_behavior(behavior::factory*)                                         = 0;
        virtual bool               contains_component(entity_id id, const Storage** storage, size_t count) const = 0;

        template <typename COMPONENT>
        COMPONENT* add_component(entity_id id)
        {
            return static_cast<COMPONENT*>(add_component(id, COMPONENT::type_info.id));
        }

        template <typename COMPONENT>
        void remove_component(entity_id id)
        {
            remove_component(id, COMPONENT::type_info.id);
        }

        template <typename COMPONENT>
        bool contains_component(entity_id id) const
        {
            return contains(id, COMPONENT::type_info.id);
        }

        template <typename COMPONENT>
        void patch_component(entity_id id)
        {
            patch_component(id, COMPONENT::type_info.id);
        }

        auto view(std::convertible_to<std::string_view> auto&&... s);
        auto view(std::convertible_to<tag_id> auto&&... s);
        template <typename T, size_t S>
        auto view(std::initializer_list<T>&& list);
    };



    class system
    {
    public:
        system()          = default;
        virtual ~system() = default;
    
        virtual void init(scene& scn){};
        virtual void deinit(scene& scn){};
        virtual void update(scene& scn, float delta) = 0;
        virtual void on_scene_begin(scene& scn){};
        virtual void on_scene_end(scene& scn){};
        virtual void on_frame_begin(scene& scn, float delta_time){};
        virtual void on_frame_end(scene& scn){};
    };



    struct behavior::factory
    {
        std::string_view id;
        std::string_view name;
        plugin*          plugin{};
        behavior* (*construct)(factory*){};
        void (*destruct)(factory*, behavior*){};
        std::pmr::unsynchronized_pool_resource pool;
    };


    template <typename BEHAVIOR>
    class behavior_base : public behavior
    {
        friend class scene;
    private:
        static behavior* type_construct(factory* f)
        {
            return new (f->pool.allocate(sizeof(BEHAVIOR))) BEHAVIOR();
        }

        static void type_destruct(factory* f, behavior* b)
        {
            if (!b)
                return;
            static_cast<BEHAVIOR*>(b)->~BEHAVIOR();
            f->pool.deallocate(b, sizeof(BEHAVIOR));
        }

        static factory* type_factory(std::string_view id, std::string_view name)
        {
            static factory s_fact;
            if (!s_fact.construct)
            {
                auto copystr = [](factory& fact, std::string_view str)
                {
                    auto strid = static_cast<char*>(fact.pool.allocate(str.size() + 1));
                    strid[str.size()] = 0;
                    std::copy(str.begin(), str.end(), strid);
                    return std::string_view(strid, str.size());
                };
                s_fact.construct = &behavior_base<BEHAVIOR>::type_construct;
                s_fact.destruct  = &behavior_base<BEHAVIOR>::type_destruct;
                s_fact.id   = copystr(s_fact, id);
                s_fact.name = copystr(s_fact, name);
            }
            return &s_fact;
        }
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
        _ent = {_view->_scene, *_ptr};
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

    template <typename B>
    inline scene& scene::register_behavior(std::string_view id, std::string_view name)
    {
        register_behavior(B::type_factory(id, name));
        return *this;
    }


    inline entity::entity(scene* scn, entity_id id) : _scene(scn), _id(id)
    {
    }

    inline entity::entity(const entity& ent) : _scene(ent._scene), _id(ent._id)
    {
    }

    inline entity& entity::operator=(const entity& ent)
    {
        _scene = ent._scene;
        _id    = ent._id;
        return *this;
    }

    inline entity::operator bool() const
    {
        return _scene && _scene->is_valid(_id);
    }

    inline bool entity::is_valid() const
    {
        return _scene && _scene->is_valid(_id);
    }

    inline entity_id entity::get_id() const
    {
        return _id;
    }

    inline scene* entity::get_scene() const
    {
        return _scene;
    }

    inline behavior* entity::get_behavior(std::string_view id) const
    {
        return _scene->get_behavior(_id, id);
    }

    inline component* entity::get_component(std::string_view id) const
    {
        return _scene->get_component(_id, id);
    }

    inline void entity::patch_component(std::string_view id)
    {
        return _scene->patch_component(_id, id);
    }

    inline bool entity::contains_behavior(std::string_view id) const
    {
        return _scene->contains_behavior(_id, id);
    }

    inline bool entity::contains_component(std::string_view id) const
    {
        return _scene->contains_component(_id, id);
    }

    inline bool entity::contains_tag(tag_id id) const
    {
        return _scene->contains_tag(_id, id);
    }

    inline behavior* entity::add_behavior(std::string_view id)
    {
        _scene->add_behavior(_id, id);
        return _scene->get_behavior(_id, id);
    }

    inline component* entity::add_component(std::string_view id)
    {
        _scene->add_component(_id, id);
        return _scene->get_component(_id, id);
    }

    inline void entity::add_tag(tag_id id)
    {
        _scene->add_tag(_id, id);
    }

    inline void entity::remove_behavior(std::string_view id)
    {
        _scene->remove_behavior(_id, id);
    }

    inline void entity::remove_component(std::string_view id)
    {
        _scene->remove_component(_id, id);
    }

    inline void entity::remove_tag(tag_id id)
    {
        _scene->remove_tag(_id, id);
    }
} // namespace box