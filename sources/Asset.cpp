#include "Asset.hpp"
#include "Renderer.hpp"
#include "Plugin.hpp"

namespace box
{
    template <typename ASSET, typename BASE>
    asset_ref<BASE> asset_provider_impl::load_asset(const char* path)
    {
        auto ret = new ASSET();
        ret->set_asset_provider(this);
        ret->load(path);
        return ret;
    }

	asset_provider_impl::asset_provider_impl()
    {
        _root._name = "root";
        _active.emplace_back(&_root);
	}

	asset_provider_impl::~asset_provider_impl()
	{
	}

	void asset_provider_impl::init(renderer* renderer)
	{
		_renderer = renderer;
	}

	asset_ref<texture> asset_provider_impl::load_texture(const char* path)
    {
        return load_asset<texture_impl, texture>(path);
	}

	asset_ref<material> asset_provider_impl::load_material(const char* path)
    {
        return load_asset<material_impl, material>(path);
	}

	asset_ref<sound> asset_provider_impl::load_sound(const char* path)
	{
		return asset_ref<sound>();
	}

	asset_ref<prototype> asset_provider_impl::load_prototype(const char* path)
	{
		return asset_ref<prototype>();
    }

    asset_ref<asset> asset_provider_impl::load(uint64_t uuid)
    {
        return asset_ref<asset>();
    }

    void asset_provider_impl::on_imgui()
    {
        ImGui::SetNextWindowPos(ImVec2(500, 800), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(950, 250), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Asset explorer"))
        {
            ImGui::End();
            return;
        }

        if(ImGui::Button(" + Add "))
        {
            auto files = open_file_dialog("", true);
            if (files.size())
            {
                for (auto pth: files)
                {
                    std::filesystem::path path(pth);
                    if (path.has_filename())
                    {
                        node n;
                        n._name = (const char*)path.filename().u8string().c_str();
                        active_node()->_items.emplace(n);
                    }
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(" + Dir "))
        {
            node n;
            n._name = std::to_string(rand());
            active_node()->_items.emplace(n);
        }
        ImGui::SameLine();
        if (ImGui::Button(" - Del "))
        {
            delete_node(_selected);
            _selected = nullptr;
        }

        ImGui::SameLine(0, 32);

        for (auto& el : _active)
        {
            if(ImGui::SmallButton(el->_name.c_str()))
            {
                while (el != _active.back())
                {
                    _active.pop_back();
                }
                break;
            }
            if (el != _active.back())
                ImGui::SameLine();
        }

        if (ImGui::BeginChildFrame(1, {0, 0}))
        {
            on_imgui_node(active_node());
        }
        ImGui::EndChildFrame();

        ImGui::End();
    }

    void asset_provider_impl::on_imgui_node(node* active)
    {
        auto&  style = ImGui::GetStyle();
        ImVec2 button_sz(128, 0);
        float  window_visible_x2 = ImGui::GetWindowPos().x + ImGui::GetWindowContentRegionMax().x;

        if (active != &_root)
        {
            if (ImGui::Button("..", button_sz))
            {
                _active.pop_back();
            }
            ImGui::SameLine();
        }

        for (auto& el : active->_items)
        {
            ImGui::PushID(&el);
            {
                if(ImGui::Button(el._name.c_str(), button_sz))
                {
                    select_node(&el);
                }
                if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
                {
                    activate_node(&el);
                }
                ImVec2 rcmin = ImGui::GetItemRectMin();
                ImVec2 rcmax = ImGui::GetItemRectMax();
                if (_selected == &el)
                {
                    ImGui::GetWindowDrawList()->AddRect(rcmin,
                                                        rcmax,
                                                        ImGui::ColorConvertFloat4ToU32(style.Colors[ImGuiCol_DockingPreview]),
                                                        style.FrameRounding,0,2);
                }
                float next_button_x2 = rcmax.x + style.ItemSpacing.x + button_sz.x;
                if (next_button_x2 < window_visible_x2)
                    ImGui::SameLine();
            }
            ImGui::PopID();
        }
    }

    asset_provider_impl::node* asset_provider_impl::active_node()
    {
        return _active.back();
    }

    void asset_provider_impl::delete_node(node* target)
    {
        if (!target || target == &_root)
            return;
        
        // Check active folder first
        auto* active = active_node();
        auto it = active->_items.find(*target);
        if (it != active->_items.end())
        {
            active->_items.erase(it);
            return;
        }

        // Search whole tree
        std::vector<node*> stack;
        stack.push_back(&_root);

        for (size_t n = 0; n < stack.size(); ++n)
        {
            auto it = stack[n]->_items.find(*target);
            if (it != stack[n]->_items.end())
            {
                stack[n]->_items.erase(it);
                return;
            }
            if (!stack[n]->_data)
            {
                for (auto& it : stack[n]->_items)
                {
                    stack.push_back(const_cast<node*>(&it));
                }
            }
        }
    }

    void asset_provider_impl::select_node(const node* n)
    {
        _selected = const_cast<node*>(n);
    }

    void asset_provider_impl::activate_node(const node* n)
    {
        if (!n)
            return;
        if (!n->_data)
        {
            _active.emplace_back(const_cast<node*>(n));
            _selected = nullptr;
        }
    }

    std::string asset_provider_impl::generate_unique_name(const node* n, std::string_view name) const
    {
        node        nde;
        nde._name = name;
        int i     = 0;
        while (n->_items.find(nde) != n->_items.end())
        {
            nde._name = name;
            nde._name += std::to_string(++i);
        }
        return std::move(nde._name);
    }

    bool asset_provider_impl::asset_data::operator<(const asset_data& d) const
    {
        return _uuid < d._uuid;
    }

    bool asset_provider_impl::node::operator<(const node& d) const
    {
        return _name < d._name;
    }
} // namespace box