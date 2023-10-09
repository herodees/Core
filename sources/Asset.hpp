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

		AssetRef<ITexture> load_texture(const char* path) override;
		AssetRef<IMaterial> load_material(const char* path) override;
		AssetRef<ISound> load_sound(const char* path) override;
		AssetRef<IPrototype> load_prototype(const char* path) override;

	private:
		Renderer* _renderer{};
	};
}