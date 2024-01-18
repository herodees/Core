#pragma once

#include "Include.hpp"

namespace box
{
    class renderer_impl;

    class asset_provider_impl final : public asset_provider
    {
        struct rename_node_dialog;
    public:

        struct asset_data
        {
            uint64_t         _uuid{};
            char             _type[8]{};
            asset_ref<asset> _asset;
        };

        struct node
        {
            asset_data*    _data{};
            node*          _parent{};
            std::string    _name;
            std::set<node> _items{};
            bool           operator<(const node& d) const;
        };

        asset_provider_impl(game& gme);
        ~asset_provider_impl() override;

        std::string_view path() const;
        bool             load(const char* path);
        bool             save(const char* path = nullptr);

        void init(renderer* renderer);

        asset_ref<texture>   load_texture(const char* path) override;
        asset_ref<material>  load_material(const char* path) override;
        asset_ref<sound>     load_sound(const char* path) override;
        asset_ref<prototype> load_prototype(const char* path) override;

        template <typename ASSET, typename BASE>
        asset_ref<BASE> load_asset(const char* path);

        asset_ref<asset> load(uint64_t uuid);

        void on_imgui();

    private:
        void        on_imgui_node(node* active);
        node*       active_node();
        void        delete_node(node* n);
        void        select_node(const node* n);
        void        activate_node(const node* n);
        std::string generate_unique_name(const node* n, std::string_view name) const;
        bool        load_filter(node* n, var& ar);
        bool        save_filter(const node* n, var& ar);
        void        get_unique_name(const asset_data& ad, std::string& out, bool full) const;
        void        add_texture(node* n);
        void        add_sprite(node* n);
        bool add_asset(node* n, std::string_view filename, std::string_view ext, const void* data, size_t size);

        renderer*                         _renderer{};
        node                              _root;
        std::set<asset_data, std::less<>> _nodes;
        std::vector<node*>                _active;
        std::string                       _path;
        node*                             _selected{};
        game&                             _game;
    };

    
    inline bool operator<(const asset_provider_impl::asset_data& c, uint64_t x)
    {
        return c._uuid < x;
    }
    inline bool operator<(uint64_t x, const asset_provider_impl::asset_data& c)
    {
        return x < c._uuid;
    }
    inline bool operator<(const asset_provider_impl::asset_data& c, const asset_provider_impl::asset_data& d)
    {
        return c._uuid < d._uuid;
    }
} // namespace box