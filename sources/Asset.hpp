#pragma once

#include "Include.hpp"

namespace box
{
    class renderer_impl;

    class asset_provider_impl final : public asset_provider
    {
        struct asset_data
        {
            uint64_t         _uuid{};
            uint32_t         _type{};
            asset_ref<asset> _asset;
            bool             operator<(const asset_data& d) const;
        };
        struct node
        {
            asset_data*                  _data{};
            std::string                  _name;
            std::set<node>               _items{};
            bool                         operator<(const node& d) const;
        };
    public:
        asset_provider_impl();
        ~asset_provider_impl() override;

        void init(renderer* renderer);

        asset_ref<texture> load_texture(const char* path) override;
        asset_ref<material> load_material(const char* path) override;
        asset_ref<sound> load_sound(const char* path) override;
        asset_ref<prototype> load_prototype(const char* path) override;

        template <typename ASSET, typename BASE>
        asset_ref<BASE> load_asset(const char* path);

        asset_ref<asset> load(uint64_t uuid);

        void on_imgui();

    private:
        void  on_imgui_node(node* active);
        node* active_node();
        void  delete_node(node* n);

        renderer* _renderer{};
        node      _root;
        std::set<asset_data> _nodes;
        std::vector<node*>   _active;
        node*                _selected{};
    };
}