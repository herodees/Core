#include "Asset.hpp"
#include "Renderer.hpp"
#include "Plugin.hpp"
#include "Editor.hpp"
#include "Imgui.hpp"

namespace box
{
    struct asset_provider_impl::rename_node_dialog : string_dialog
    {
        rename_node_dialog()
        {
        
        }

        bool validate(std::string_view value) override
        {
            if (value.empty() || value == _target->_name)
                return false;

            if (!_target->_parent)
                return true;

            node nde;
            nde._name = value;
            auto it = _target->_parent->_items.find(nde);
            return it == _target->_parent->_items.end();
        }

        asset_provider_impl::node* _target{};
    };


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
        _root._name = "game:";
        _active.emplace_back(&_root);
	}

	asset_provider_impl::~asset_provider_impl()
	{
    }

    bool asset_provider_impl::load(const char* path)
    {
        _path = path;
        _root._items.clear();
        _nodes.clear();

        auto data = ray::LoadFileText(path);
        if (!data)
            return false;

        var doc;
        auto r = doc.from_string(data);
        ray::MemFree(data);

        if(var_error::ok != r)
            return false;

        var assets = doc.get_item("assets");
        for (auto& el : assets.elements())
        {
            asset_data data;
            data._uuid = el.get_item("id").get(0ull);
            strcpy(data._type, el.get_item("t").c_str());
            _nodes.insert(data);
        }

        var filter = doc.get_item("filter");
        return load_filter(&_root, filter);
    }

    bool asset_provider_impl::save(const char* path)
    {
        if (path)
            _path = path;

        var assets;
        assets.make_array(uint32_t(_nodes.size()));
        for (auto& n : _nodes)
        {
            var item;
            item.set_item("id", n._uuid);
            item.set_item("t", n._type);
            assets.push_back(item);
        }

        var filter;
        if (!save_filter(&_root, filter))
            return false;

        var doc;
        doc.set_item("assets", assets);
        doc.set_item("filter", filter);

        std::string str;
        if (!doc.to_string(str))
            return false;

        return ray::SaveFileText(_path.c_str(), str.data());
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

    std::string_view asset_provider_impl::path() const
    {
        return _path;
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

        if (ImGui::BeginCombo("##dd", " + Add ", ImGuiComboFlags_NoArrowButton | ImGuiComboFlags_WidthFitPreview))
        {
            if (ImGui::MenuItem("Folder",nullptr,nullptr,true))
            {
                node n;
                n._name = std::to_string(generate_uuid());
                n._parent = active_node();
                active_node()->_items.emplace(n);
            }
            ImGui::Separator();
            if (ImGui::MenuItem("Texture", nullptr, nullptr, true))
            {
                add_texture(active_node());
            }
            if (ImGui::MenuItem("Sprite", nullptr, nullptr, true))
            {
                add_sprite(active_node());
            }

            ImGui::EndCombo();
        }
        ImGui::SameLine();
        if(ImGui::Button(" + Add "))
        {
            auto files = open_file_dialog("", true);
            if (files.size())
            {
                std::string fullpath;
                for (auto pth: files)
                {
                    std::string_view name = ray::GetFileName(pth.c_str());
                    if (!name.empty())
                    {
                        std::string_view       ext = ray::GetFileExtension(name.data());
                        asset_data data;
                        node       n;
                        data._uuid = generate_uuid();
                        std::copy(ext.begin(), ext.begin() + std::min(ext.size(), std::size(data._type)), data._type);
                        n._name = name;
                        n._parent = active_node();

                        int32_t size{};
                        if (auto fd = ray::LoadFileData(pth.c_str(), &size))
                        {
                            get_unique_name(data, fullpath, true);

                            if (ray::SaveFileData(fullpath.c_str(), fd, size))
                            {
                                n._data = const_cast<asset_data*>(&(*_nodes.emplace(data).first));
                                active_node()->_items.emplace(n);
                            }

                            ray::MemFree(fd);
                        }
                    }
                }
            }
        }
        ImGui::SameLine();
        if (ImGui::Button(" - Del "))
        {
            delete_node(_selected);
            _selected = nullptr;
        }
        ImGui::SameLine();
        if (ImGui::Button(" Save "))
        {
            save();
        }

        ImGui::SameLine(0, 32);

        for (auto& el : _active)
        {
            ImGui::PushID(el);
            if(ImGui::SmallButton(el->_name.c_str()))
            {
                while (el != _active.back())
                {
                    _active.pop_back();
                }
                ImGui::PopID();
                break;
            }
            if (el != _active.back())
            {
                ImGui::SameLine();
                ImGui::Text("/");
                ImGui::SameLine();
            }
              
            ImGui::PopID();
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
            ImGui::Button("..", button_sz);
            if (ImGui::IsMouseDoubleClicked(0) && ImGui::IsItemHovered())
            {
                _active.pop_back();
            }
            ImGui::SameLine();
        }

        for (auto& el : active->_items)
        {
            ImGui::PushID(&el);
            {
                if (!el._data)
                {
                    ImGui::PushStyleColor(ImGuiCol_Button, ImGui::GetStyleColorVec4(ImGuiCol_TitleBg));
                }
                if(ImGui::Button(el._name.c_str(), button_sz))
                {
                    select_node(&el);
                }
                if (!el._data)
                {
                    ImGui::PopStyleColor();
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
        if (!target || target == &_root || !target->_parent)
            return;

        auto it = target->_parent->_items.find(*target);
        if (it != target->_parent->_items.end())
        {
            target->_parent->_items.erase(it);
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
        else
        {
            char filename[2*sizeof(asset_data::_uuid) + sizeof(asset_data::_type) + 1];
            sprintf(filename, "%016" PRIx64 "%s", n->_data->_uuid, n->_data->_type);
            open_default_application(filename);
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

    bool asset_provider_impl::load_filter(node* n, var& ar)
    {
        if (!ar.is_object())
            return false;

        if (n != &_root)
            n->_name = ar.get_item("n").str();

        auto it = _nodes.find(ar.get_item("id").get(0ull));
        if (it != _nodes.end())
        {
            n->_data = const_cast<asset_data*>(&(*it));
        }

        auto itms = ar.get_item("c");
        for (auto& el : itms.elements())
        {
            node nde;
            nde._parent = n;
            load_filter(&nde, el);
            n->_items.insert(nde);
        }

        return true;
    }

    bool asset_provider_impl::save_filter(const node* n, var& ar)
    {
        var items;
        for (auto& el : n->_items)
        {
            var arr;
            arr.set_item("n", el._name.c_str());
            if (el._data)
            {
                arr.set_item("id", el._data->_uuid);
            }
            save_filter(&el, arr);
            items.push_back(arr);
        }
        if (items.size())
        {
            ar.set_item("c", items);
        }

        return true;
    }

    void asset_provider_impl::get_unique_name(const asset_data& ad, std::string& out, bool full) const
    {
        char filename[2 * sizeof(asset_data::_uuid) + sizeof(asset_data::_type) + 1];
        sprintf(filename, "%016" PRIx64 "%s", ad._uuid, ad._type);
        if (full)
            out.assign(_path.c_str(), ray::GetFileName(_path.c_str()));
        else
            out.clear();
        out.append(filename);
    }

    void asset_provider_impl::add_texture(node* root)
    {
        auto files = open_file_dialog("", true, "Image|*.png;*.jpg;*.gif");
        if (files.size())
        {
            std::string fullpath;
            for (auto pth : files)
            {
                std::string_view name = ray::GetFileName(pth.c_str());
                if (!name.empty())
                {
                    std::string_view ext = ray::GetFileExtension(name.data());
                    int32_t          size{};
                    if (auto fd = ray::LoadFileData(pth.c_str(), &size))
                    {
                        add_asset(root, name, ext, fd, size);
                        ray::MemFree(fd);
                    }
                }
            }
        }
    }

    void asset_provider_impl::add_sprite(node* n)
    {

        auto file = save_file_dialog("", "Sprite|*.sprite");
        if (file.size())
        {
        }
    }

    bool asset_provider_impl::add_asset(
        node*            root,
        std::string_view filename,
        std::string_view ext, const void* data, size_t size)
    {
        if (!data || !size)
            return false;

        asset_data adata;
        node       n;
        adata._uuid = generate_uuid();
        std::copy(ext.begin(), ext.begin() + std::min(ext.size(), std::size(adata._type)), adata._type);
        n._name = filename;
        n._parent = root;

        std::string fullpath;
        get_unique_name(adata, fullpath, true);

        if (ray::SaveFileData(fullpath.c_str(), (void*)data, (int32_t)size))
        {
            n._data = const_cast<asset_data*>(&(*_nodes.emplace(adata).first));
            root->_items.emplace(n);
            return true;
        }
        return false;
    }

    bool asset_provider_impl::node::operator<(const node& d) const
    {
        return _name < d._name;
    }
} // namespace box