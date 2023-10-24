#include "Game.hpp"

#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Core"

namespace box
{
    struct null_pugin : plugin
    {
        null_pugin()           = default;
        ~null_pugin() override = default;
    } null_p;

	game_impl::game_impl() : _plugin(&null_p), _scene(*this)
    {
	}

	game_impl::~game_impl()
	{
	}

    renderer& game_impl::get_renderer()
    {
        return *static_cast<renderer*>(_scene.get_system("renderer"));
    }

    asset_provider& game_impl::get_asset()
    {
        return _assets;
    }

    scene& game_impl::get_scene()
    {
        return _scene;
    }

    physics& game_impl::get_physics()
    {
        return *static_cast<physics*>(_scene.get_system("physics"));
    }

    inputs& game_impl::get_inputs()
    {
        return _inputs;
    }

    video& game_impl::get_video()
    {
        return _video;
    }

	plugin& game_impl::get_plugin()
	{
        return *_plugin;
    }

    void game_impl::on_frame_begin()
    {
        get_plugin().on_frame_begin(*this, ray::GetFrameTime());
        _scene.on_frame_begin(ray::GetFrameTime());
    }

    void game_impl::on_frame_end()
    {
        _scene.on_frame_end();
        get_plugin().on_frame_end(*this);
    }

    void game_impl::on_update()
    {
        get_plugin().on_step(*this);
        _scene.update(ray::GetFrameTime());
    }

    int32_t game_impl::run(const char* v[], int32_t c)
    {
    //    ray::SetConfigFlags(ray::FLAG_MSAA_4X_HINT );
        ray::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
        ray::SetTargetFPS(60);

        init(v, c);
        _scene.init();
        _assets.init(&get_renderer());

        get_plugin().on_init(*this);
        while (!ray::WindowShouldClose())
        {
            on_frame_begin();
            on_update();
            on_frame_end();
        }
        get_plugin().on_deinit(*this);
        _scene.deinit();

		ray::CloseWindow();
        return 0;
	}
    struct testt : public behavior_base<testt>
    {
    };
    void game_impl::init(const char* v[], int32_t c)
    {
		if (c == 2)
		{
            _game_plugin.load(v[2]);
		}
		else
		{
            _game_plugin.load("data/game");
		}

		if (_game_plugin.get())
		{
            _plugin = _game_plugin.get();
		}
        else
        {
            _plugin = &null_p;
		}


        

        get_scene().register_behavior<testt>("tst", "Test");
    }

}