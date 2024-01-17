#include "Game.hpp"
#include "Editor.hpp"

#if defined(PLATFORM_WEB)
#include <emscripten/emscripten.h>
#endif

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

    void game_impl::setup(const char* v[], int32_t c)
    {
        _argv = v;
        _argc = c;
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

    imgui& game_impl::get_imgui()
    {
        return _imgui;
    }

    void game_impl::on_frame_begin()
    {
        _imgui.on_frame_begin();
        get_plugin().on_frame_begin(*this, ray::GetFrameTime());
        _scene.on_frame_begin(ray::GetFrameTime());
    }

    void game_impl::on_frame_end()
    {
        _scene.on_frame_end();
        get_plugin().on_frame_end(*this);
        _imgui.on_frame_end();
    }

    void game_impl::on_imgui()
    {
        if (_assets.path().empty())
        {
            ImGui::SetNextWindowPos(ImVec2(500, 200), ImGuiCond_FirstUseEver);
            ImGui::SetNextWindowSize(ImVec2(950, 500), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Project"))
            {
                ImGui::Dummy({255, 100});
                ImGui::Dummy({255, 100});
                ImGui::SameLine();
                if (ImGui::Button("Open project", {200,64}))
                {
                    auto file = open_file_dialog("main.core", false, "Core Project|*.core");
                    if (!file.empty())
                    {
                        _assets.load(file.back().c_str());
                    }
                }
                ImGui::SameLine();
                if (ImGui::Button("New project", {200, 64}))
                {
                    auto file = save_file_dialog("main.core", "Core Project|*.core");
                    if (!file.empty())
                    {
                        _assets.save(file.c_str());
                    }
                }
            }
            ImGui::End();
        }
        else
        {
            get_plugin().on_imgui(*this);
            _scene.on_imgui();
            _assets.on_imgui();
        }
    }

    void game_impl::on_update()
    {
        get_plugin().on_step(*this);
        _scene.update(ray::GetFrameTime());
    }

    void game_impl::em_arg_callback_func(void* p)
    {
        game_impl* self = (game_impl*)(p);
        ray::BeginDrawing();
        self->on_frame_begin();
        self->on_update();
        self->on_imgui();
        self->on_frame_end();
        ray::EndDrawing();
    }

    int32_t game_impl::run( )
    {

    //    ray::SetConfigFlags(ray::FLAG_MSAA_4X_HINT );
        ray::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
        ray::SetTargetFPS(60);

        init( );
        _scene.init();
        _assets.init(&get_renderer());
        _imgui.init(true);

        get_plugin().on_init(*this);

#if defined(PLATFORM_WEB)
        emscripten_set_main_loop_arg(game_impl::em_arg_callback_func, this, 0, 1);
#else
        while (!ray::WindowShouldClose())
        {
            em_arg_callback_func(this);
        }
#endif
        
        get_plugin().on_deinit(*this);
        _scene.deinit();
        _imgui.deinit();

		ray::CloseWindow();
        return 0;
	}

    void game_impl::init( )
    {
		if (_argc == 2)
        {
            _assets.load(_argv[1]);
		}
		else
		{
     //       _assets.load("data/main.core");
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