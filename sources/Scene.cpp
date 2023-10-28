#include "Scene.hpp"
#include "Physics.hpp"
#include "Renderer.hpp"
#include "Tileset.hpp"

namespace box
{


    scene_impl::scene_impl(game& gme) : _game(gme)
    {
    }

    scene_impl::~scene_impl()
    {
        _registry.clear();
        _systems.clear();
        _behaviors.clear();
    }

    void scene_impl::init()
    {
        register_system<physics_impl>();
        register_system<renderer_impl>();
        register_system<tileset_impl>();

        for (auto& it : _systems)
        {
            it.second->init(*this);
        }

        _game.get_physics().set_gravity({0, 300});
        _game.get_physics().set_iterations(1);
        _game.get_physics().set_sleep_time_treshold(0.5f);
        _game.get_physics().set_collision_slop(0.5f);

        auto item_create = [&](float x, float y, float r, float fr, bool stat)
        {
            auto ent = create();

            auto body = ent.add_component(rigid_body_component::type_info.id)->as<rigid_body_component>();
            body->set_position({x, y});
            body->set_moment(0.1);
            body->set_mass(fr);
            body->set_type(stat ? body_type::STATIC : body_type::DYNAMIC);

            auto collider = ent.add_component(circle_collider_component::type_info.id)->as<circle_collider_component>();
            collider->set_body(body);
            collider->setup(r, {0, 0});
            collider->set_elasticity(0.699);
            collider->set_friction(1.f);
          
        };
        item_create(461, 50, 20, 10, false);
        item_create(501, 50, 20, 10, false);
        item_create(500, 200, 40, 5, true);
        item_create(450, 300, 40, 5, true);
        item_create(650, 300, 40, 5, true);

        for (int i = 0; i < 100000; ++i)
        {
            auto ent = create();
        }
    }

    void scene_impl::deinit()
    {
        for (auto& it : _systems)
        {
            it.second->deinit(*this);
        }
    }

    entity scene_impl::create()
    {
        auto ent = _registry.create();
        return entity(this, (entity_id)ent);
    }

    void scene_impl::release(entity_id id)
    {
        for (auto& it : _behaviors)
            it.second.storage.remove((entt::entity)id);

        for (auto& it : _tags)
            it.remove((entt::entity)id);

        _registry.destroy((entt::entity)id);
    }

    bool scene_impl::is_valid(entity_id id) const
    {
        return _registry.valid((entt::entity)id);
    }

    component* scene_impl::get_component(entity_id id, std::string_view cmp_id) 
    {
        if (auto* cmp = find_component_definition(cmp_id))
        {
            return static_cast<component*>(cmp->storage->get((entt::entity)id));
        }
        return nullptr;
    }

    component* scene_impl::add_component(entity_id id, std::string_view cmp_id)
    {
        if (auto* cmp = find_component_definition(cmp_id))
        {
            cmp->storage->emplace((entt::entity)id);
            return static_cast<component*>(cmp->storage->get((entt::entity)id));
        }
        return nullptr;
    }

    void scene_impl::remove_component(entity_id id, std::string_view cmp_id)
    {
        if (auto* cmp = find_component_definition(cmp_id))
        {
            cmp->storage->remove((entt::entity)id);
        }
    }

    bool scene_impl::contains_component(entity_id id, std::string_view cmp_id) const
    {
        if (auto* cmp = find_component_definition(cmp_id))
        {
            return cmp->storage->contains((entt::entity)id);
        }
        return false;
    }

    bool scene_impl::contains_component(entity_id id, const Storage** storage, size_t count) const
    {
        const entt::sparse_set** sets = reinterpret_cast<const entt::sparse_set**>(storage);
        for (size_t n = 0; n < count; ++n)
        {
            if (!sets[n]->contains((entt::entity)id))
                return false;
        }
        return true;
    }

    void scene_impl::patch_component(entity_id id, std::string_view cmp_id)
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

    void scene_impl::add_behavior(entity_id id, std::string_view beh_id)
    {
        auto it = _behaviors.find(beh_id);
        if (it == _behaviors.end())
            return;
        it->second.storage.emplace((entt::entity)id);
    }

    void scene_impl::remove_behavior(entity_id id, std::string_view beh_id)
    {
        auto it = _behaviors.find(beh_id);
        if (it == _behaviors.end())
            return;
        it->second.storage.remove((entt::entity)id);
    }

    bool scene_impl::contains_behavior(entity_id id, std::string_view beh_id) const
    {
        auto it = _behaviors.find(beh_id);
        if (it == _behaviors.end())
            return false;
        return it->second.storage.contains((entt::entity)id);
    }

    behavior* scene_impl::get_behavior(entity_id id, std::string_view beh_id)
    {
        auto it = _behaviors.find(beh_id);
        if (it == _behaviors.end())
            return nullptr;
        return it->second.storage.get((entt::entity)id);
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

    game& scene_impl::get_game() const
    {
        return _game;
    }
    
    behavior::factory* scene_impl::register_behavior(behavior::factory* f)
    {
        auto& def = _behaviors[std::string(f->id)];
        def.factory = f;
        return f;
    }

    const component_definition* scene_impl::find_component_definition(std::string_view id) const
    {
        auto it = _components.find(id);
        if (it == _components.end())
            return nullptr;
        return &it->second;
    }

    void scene_impl::update(float delta_time)
    {
        for (auto& it : _systems)
        {
            it.second->update(*this, delta_time);
        }
    }

    void scene_impl::on_frame_begin(float delta_time)
    {
        for (auto& it : _systems)
        {
            it.second->on_frame_begin(*this, delta_time);
        }
    }

    void scene_impl::on_frame_end()
    {
        for (auto& it : _systems)
        {
            it.second->on_frame_end(*this);
        }
    }

    void scene_impl::on_imgui()
    {
        for (auto& it : _systems)
        {
            it.second->on_imgui(*this);
        }

        show_overlay_imgui();
        show_scene_imgui();
        show_entity_imgui(_active_entity);
    }

    entt::registry& scene_impl::get_registry()
    {
        return _registry;
    }

    void scene_impl::show_scene_imgui()
    {
        ImGui::SetNextWindowPos(ImVec2(32, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(350, 680), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Scene inspector"))
        {
            ImGui::End();
            return;
        }

        if (ImGui::BeginChildFrame(1, {0, 0}))
        {
            char str[32];

            ImGuiListClipper clipper;
            clipper.Begin(_registry.size());
            while (clipper.Step())
            {
                auto it = _registry.data() + clipper.DisplayStart;
                for (auto row = clipper.DisplayStart; row < clipper.DisplayEnd; row++)
                {
                    const uint32_t e = entt::to_integral(*it);
                    sprintf(str, "%08X", e);
                    if (ImGui::Selectable(str, _active_entity == row))
                        _active_entity = row;
                    ++it;
                }
            }
        }
        ImGui::EndChildFrame();

        ImGui::End();
    }

    void scene_impl::show_entity_imgui(int32_t index)
    {
        constexpr auto width = 350;
        ImGui::SetNextWindowPos(ImVec2(ImGui::GetIO().DisplaySize.x - width - 32, 100), ImGuiCond_FirstUseEver);
        ImGui::SetNextWindowSize(ImVec2(width, 680), ImGuiCond_FirstUseEver);
        if (!ImGui::Begin("Entity properties") || index >= _registry.size() || !_registry.valid(_registry.data()[index]))
        {
            ImGui::End();
            return;
        }
        auto eid = _registry.data()[index];

        for (auto storage : _components)
        {
            if (storage.second.storage->contains(eid))
            {
                bool visible = true;
                if (ImGui::CollapsingHeader(storage.second.name.c_str(), &visible, ImGuiTreeNodeFlags_DefaultOpen))
                {

                    component* cmp = static_cast<component*>(storage.second.storage->get(eid));
                    entity     ent(this, (entity_id)eid);
                    cmp->on_edit(ent);
                }
                if (!visible)
                {
                    storage.second.storage->remove(eid);
                }
            }
        }

        ImGui::NewLine();
        ImGui::NewLine();
        ImGui::SameLine((ImGui::GetContentRegionAvail().x - 120) / 2);
        
        ImGui::SetNextItemWidth(120);
        if (ImGui::BeginCombo("##add", "Add component", ImGuiComboFlags_PopupAlignLeft))
        {
            if (ImGui::BeginMenu("Components"))
            {
                for (auto& el : _components)
                {
                    ImGui::MenuItem(el.second.name.c_str(), nullptr, nullptr, !el.second.storage->contains(eid));
                }
                ImGui::EndMenu();
            }
            if (ImGui::BeginMenu("Behaviors"))
            {
                for (auto& el : _behaviors)
                {
                    ImGui::MenuItem(el.second.factory->name.data(), nullptr, nullptr, !el.second.storage.contains(eid));
                }
                ImGui::EndMenu();
            }
            ImGui::EndCombo();
        }



        ImGui::End();
    }

    void scene_impl::show_overlay_imgui()
    {
        ImGuiWindowFlags window_flags = ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_NoDocking |
                                        ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings |
                                        ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav;

        ImGui::SetNextWindowPos({ImGui::GetMainViewport()->GetCenter().x, 32}, ImGuiCond_Always, ImVec2(0.5f, 0.5f));
        ImGui::SetNextWindowBgAlpha(0.95f); // Transparent background
        if (ImGui::Begin("overlay", nullptr, window_flags))
        {
            ImGui::Button(">");
            ImGui::SameLine();
            ImGui::Button("||");
            ImGui::SameLine();
            ImGui::Button("o");
        }
        ImGui::End();
    }

}