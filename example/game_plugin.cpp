#include <Core.hpp>

namespace box
{
    class test_behavior : public behavior_base<test_behavior>
    {
    public:
        void on_step(float dt) override
        {
        
        }

        void on_render() override
        {

        }
    };

    class example_plugin : public plugin
    {
    public:
        ~plugin() override = default;

        void on_init(game& gme) override
        {
            gme.get_scene().register_behavior<test_behavior>("test", "Test");
        }

        void on_deinit(game& gme) override
        {
        }

        void on_frame_begin(game& gme, float delta_time) override
        {
        }

        void on_frame_end(game& gme) override
        {
        }

        void on_scene_begin(game& gme) override
        {
            auto  ent       = gme.get_scene().create();
            auto* test_beh  = gme.get_scene().add_behavior(ent, "test")->as<test_behavior>();
            auto* rigidbody = gme.get_scene().add_component(ent, "rigidbody")->as<rigid_body_component>();
        }

        void on_scene_end(game& gme) override
        {
        }
    };



    /***************************************
    *             Plugin Entry
    ***************************************/
    DLIB_EXPORT plugin* plugin_main(game* game)
    {
        return new example_plugin();
    }

} // namespace box


