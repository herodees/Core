#pragma once

#include "Scene.hpp"
#include "Asset.hpp"
#include "Renderer.hpp"

namespace box
{
	class IGame
	{
	public:
		virtual ~IGame() = default;

		virtual IRenderer& renderer() = 0;
		virtual IAssetProvider& asset() = 0;
		virtual IScene& scene() = 0;
	};
}