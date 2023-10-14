#include <Core.hpp>

namespace box
{
    class example_plugin : public plugin
    {
    public:
        ~plugin() override = default;

        void on_init(game& gme) override{};
        void on_deinit(game& gme) override{};

        void on_frame_begin(game& gme, float delta_time) override{};
        void on_frame_end(game& gme) override{};

        void on_scene_begin(game& gme) override{};
        void on_scene_end(game& gme) override{};
    };



    /***************************************
    *             Plugin Entry
    ***************************************/
    DLIB_EXPORT plugin* plugin_main(game* game)
    {
        return new example_plugin();
    }

} // namespace box


