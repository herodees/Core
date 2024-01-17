#pragma once

#include "Vec.hpp"

namespace box
{

    class asset
    {
    public:
        virtual ~asset()                    = default;
        virtual bool save(const char* path) = 0;
        virtual bool load(const char* path) = 0;

    public:
        void add_ref()
        {
            ++_ref;
        }

        bool rem_ref()
        {
            if (!--_ref)
            {
                delete this;
                return true;
            }
            return false;
        }

        asset_provider* get_asset_provider() const
        {
            return _provider;
        }

        void set_asset_provider(asset_provider* pr)
        {
            _provider = pr;
        }

    protected:
        std::atomic_int _ref{1};
        asset_provider* _provider{};
    };


    class material : public asset
    {
    public:
        virtual ~material()                                                                             = default;
        virtual void     bind(bool activate)                                                            = 0;
        virtual void     draw(const vertex* vtx, size_t size)                                           = 0;
        virtual void     set_shader(const char* vs, const char* fs)                                     = 0;
        virtual void     set_blend_mode(blend_mode blend)                                               = 0;
        virtual void     set_texture(uint32_t loc, const texture* texture)                              = 0;
        virtual void     set_uniform(uint32_t loc, const void* data, uint32_t count, uniform_type type) = 0;
        virtual uint32_t get_location(const char* name) const                                           = 0;
    };


    class sound : public asset
    {
    public:
        virtual ~sound() = default;
    };


    class prototype : public asset
    {
    public:
        virtual ~prototype() = default;
    };


    template <typename AST>
    class asset_ref
    {
    public:
        asset_ref() = default;
        asset_ref(AST* a) : _ptr(a)
        {
        }
        asset_ref(const asset_ref<AST>& ref) : _ptr(ref._ptr)
        {
            if (_ptr)
                _ptr->add_ref();
        }
        asset_ref(asset_ref<AST>&& ref) : _ptr(ref._ptr)
        {
            ref._ptr = nullptr;
        }
        ~asset_ref()
        {
            reset();
        }
        asset_ref<AST>& operator=(const asset_ref<AST>& c)
        {
            if (c._ptr != _ptr)
                reset(c._ptr);
            return *this;
        }
        asset_ref<AST>& operator=(asset_ref<AST>&& c)
        {
            std::swap(_ptr, c._ptr);
            return *this;
        }
        void reset(AST* ptr = nullptr)
        {
            if (_ptr)
                _ptr->rem_ref();
            _ptr = ptr;
            if (_ptr)
                _ptr->add_ref();
        }

        AST& operator*()
        {
            return *_ptr;
        }
        const AST& operator*() const
        {
            return *_ptr;
        }
        AST* operator->()
        {
            return _ptr;
        }
        const AST* operator->() const
        {
            return _ptr;
        }

        operator AST*()
        {
            return _ptr;
        }
        operator const AST*() const
        {
            return _ptr;
        }
        AST* get() const
        {
            return _ptr;
        };

        AST* _ptr{};
    };


    class asset_provider
    {
    public:
        virtual ~asset_provider() = default;

        virtual asset_ref<texture>   load_texture(const char* path)   = 0;
        virtual asset_ref<material>  load_material(const char* path)  = 0;
        virtual asset_ref<sound>     load_sound(const char* path)     = 0;
        virtual asset_ref<prototype> load_prototype(const char* path) = 0;
    };

} // namespace box