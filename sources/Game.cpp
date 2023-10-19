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
        return _renderer;
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

    int32_t game_impl::run(const char* v[], int32_t c)
    {
        ray::SetConfigFlags(ray::FLAG_MSAA_4X_HINT | ray::FLAG_VSYNC_HINT);
        ray::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
      //  ray::SetTargetFPS(60);

        init(v, c);
        _renderer.init();
        _assets.init(&_renderer);
        _scene.init();

        get_plugin().on_init(*this);

		{
			auto tex = _assets.load_texture(ASSETS_PATH"test.png");



			while (!ray::WindowShouldClose())
			{
                on_frame_begin();

				_scene.update(ray::GetFrameTime());


				Recti scissor(100, 100, 1000, 1000);

				ray::BeginDrawing();
				_renderer.clear_background({ 255,255,255,255 });
				_renderer.begin_2d(camera(), false);
				_renderer.enable_scissor_test(scissor);

				auto draw = [&](Vec2f pos, uint32_t d)
				{
					_renderer.set_texture(tex);
					_renderer.set_depth(d);
					auto mesh = _renderer.begin_mesh(6);

					mesh.vertex[0].position = { pos.x + 0.f, pos.y + 0.f };
					mesh.vertex[1].position = { pos.x + 0.f, pos.y + 100.f };
					mesh.vertex[2].position = { pos.x + 100.f, pos.y + 100.f };
					mesh.vertex[3].position = { pos.x + 100.f, pos.y + 0.f };

					mesh.vertex[0].tex_coord = { 0.f, 0.f };
					mesh.vertex[1].tex_coord = { 0.f, 1.f };
					mesh.vertex[2].tex_coord = { 1.f, 1.f };
					mesh.vertex[3].tex_coord = { 1.f, 0.f };

					mesh.vertex[0].color = { 255,255,255,255 };
					mesh.vertex[1].color = { 255,255,255,255 };
					mesh.vertex[2].color = { 255,255,255,255 };
					mesh.vertex[3].color = { 255,255,255,255 };

					mesh.vertex[4] = mesh.vertex[0];
					mesh.vertex[5] = mesh.vertex[2];

					mesh.vertex_size += 6;

					_renderer.end_mesh(mesh);
				};

				for (int i = 0; i < 1000; ++i)
				{
					draw({ i * 50.f, i * 50.f }, 1000 - i);
				}

				_renderer.end_2d();

				ray::DrawFPS(10, 10);

				_renderer.draw_circle_segment({800, 800}, 60, {255, 25, 25, 255}, 0);
				_renderer.draw_rectangle({500, 400, 700, 480}, {255, 25, 255, 255});
                _renderer.draw_ellipse({300, 500, 400, 550}, {25, 25, 255, 255});

				ray::EndDrawing();

				on_frame_end();
			}
		}

        get_plugin().on_deinit(*this);
		_renderer.deinit();

		ray::CloseWindow();

        return 0;
	}

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
    }

}