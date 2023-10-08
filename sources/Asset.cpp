#include "Asset.hpp"
#include "Renderer.hpp"

namespace box
{
	AssetProvider::AssetProvider()
	{
	}

	AssetProvider::~AssetProvider()
	{
	}

	void AssetProvider::init(Renderer* renderer)
	{
		_renderer = renderer;
	}

	AssetRef<IAsset> AssetProvider::load_texture(const char* path)
	{
		return _renderer->load_texture_asset(path);
	}

	AssetRef<IAsset> AssetProvider::load_material(const char* path)
	{
		return _renderer->load_material_asset(path);
	}
}