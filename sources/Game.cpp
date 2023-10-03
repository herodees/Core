#include "Game.hpp"
#include "Game.hpp"
#include "Game.hpp"


#define SCREEN_WIDTH (1027)
#define SCREEN_HEIGHT (768)

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

        ray::Texture2D texture = ray::LoadTexture(ASSETS_PATH"test.png"); // Check README.md for how this works

        while (!ray::WindowShouldClose())
        {
            _renderer.begin_drawing();
            _renderer.clear_background({ 255,255,255,255 });

            const int texture_x = SCREEN_WIDTH / 2 - texture.width / 2;
            const int texture_y = SCREEN_HEIGHT / 2 - texture.height / 2;
            DrawTexture(texture, texture_x, texture_y, ray::WHITE);

            const char* text = "OMG! IT WORKS!";
            const ray::Vector2 text_size = ray::MeasureTextEx(ray::GetFontDefault(), text, 20, 1);
            ray::DrawText(text, SCREEN_WIDTH / 2 - text_size.x / 2, texture_y + texture.height + text_size.y + 10, 20, ray::BLACK);

            _renderer.end_drawing();
        }

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