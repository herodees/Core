#pragma once

#include "Include.hpp"
#include "Renderer.hpp"
#include "Asset.hpp"
#include "Scene.hpp"
#include "Library.hpp"

namespace box
{
	class game_impl : public game
	{
	public:
		game_impl();
		~game_impl() override;

		void init(const char* v[], int32_t c);

		renderer& get_renderer() override final;
		asset_provider& get_asset() override final;
		IScene& get_scene() override final;
		plugin* get_main() override final;

		int32_t run(const char* v[], int32_t c);

	private:
		plugin* _main{};
		renderer_impl _renderer;
		asset_provider_impl _assets;
		Scene _scene;
	};
}