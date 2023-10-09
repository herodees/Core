#pragma once

#include "Include.hpp"
#include "Renderer.hpp"
#include "Asset.hpp"
#include "Scene.hpp"

namespace box
{
	class Game : public IGame
	{
	public:
		Game();
		~Game() override;

		void init(const char* v[], int32_t c);

		IRenderer& renderer() override final;
		IAssetProvider& asset() override final;
		IScene& scene() override final;

		int32_t run(const char* v[], int32_t c);

	private:
		Renderer _renderer;
		AssetProvider _assets;
		Scene _scene;
	};
}