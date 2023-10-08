#pragma once

#include "Scene.hpp"
#include "Asset.hpp"
#include "Graphics.hpp"

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