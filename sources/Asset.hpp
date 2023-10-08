#pragma once

#include "Include.hpp"

namespace box
{
	class Renderer;

	class AssetProvider : public IAssetProvider
	{
	public:
		AssetProvider();
		~AssetProvider() override;

		void init(Renderer* renderer);

		AssetRef<IAsset> load_texture(const char* path) override;
		AssetRef<IAsset> load_material(const char* path) override;

	private:
		Renderer* _renderer{};
	};
}