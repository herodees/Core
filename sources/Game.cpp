#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"


#define SCREEN_WIDTH (1920)
#define SCREEN_HEIGHT (1080)

#define WINDOW_TITLE "Core"

namespace box
{
	Game::Game()
	{
	}

	Game::~Game()
	{
	}

    IRenderer& Game::renderer()
    {
        return _renderer;
    }

    IAssetProvider& Game::asset()
    {
        return _assets;
    }

    IScene& Game::scene()
    {
        return _scene;
    }

    int32_t Game::run()
	{
        ray::InitWindow(SCREEN_WIDTH, SCREEN_HEIGHT, WINDOW_TITLE);
        ray::SetTargetFPS(60);

        init();

        auto texture_id = _renderer.load_texture(ASSETS_PATH"test.png");

        while (!ray::WindowShouldClose())
        {
            _renderer.begin_frame();
            _renderer.clear_background({ 255,255,255,255 });
            _renderer.begin_2d(Camera(), true);

            auto draw = [&](Vec2f pos, uint32_t d)
            {
                _renderer.setTexture(texture_id);
                _renderer.setDepth(d);
                auto mesh = _renderer.beginMesh(4, 6);

                mesh.index[0] = 0;
                mesh.index[1] = 1;
                mesh.index[2] = 2;
                mesh.index[3] = 2;
                mesh.index[4] = 3;
                mesh.index[5] = 0;

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

                mesh.index_size += 6;
                mesh.vertex_size += 4;

                _renderer.endMesh(mesh);
            };

            for (int i = 0; i < 10; ++i)
            {
                draw({ i*50.f, i*50.f }, 100-i);
            }




            _renderer.end_2d();

            ray::DrawFPS(10, 10);

            _renderer.end_frame();


        }

        _renderer.unload_texture(texture_id);

        ray::CloseWindow();

        return 0;
	}

    void Game::init()
    {
        struct str : Component<str>
        {
            std::string _str;
        };
        struct flag : Component<flag>
        {
        };

        str::info = _scene.register_component<str>("test","tst");
        flag::info = _scene.register_component<flag>("flag","flg");
    }

}