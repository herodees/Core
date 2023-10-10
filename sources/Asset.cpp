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

	AssetRef<ITexture> AssetProvider::load_texture(const char* path)
	{
		return _renderer->load_texture(path);
	}

	AssetRef<IMaterial> AssetProvider::load_material(const char* path)
	{
		return _renderer->load_material(path);
	}

	AssetRef<ISound> AssetProvider::load_sound(const char* path)
	{
		return AssetRef<ISound>();
	}

	AssetRef<IPrototype> AssetProvider::load_prototype(const char* path)
	{
		return AssetRef<IPrototype>();
	}
}