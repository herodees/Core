#include "Asset.hpp"
#include "Renderer.hpp"

namespace box
{
	asset_provider_impl::asset_provider_impl()
	{
	}

	asset_provider_impl::~asset_provider_impl()
	{
	}

	void asset_provider_impl::init(renderer* renderer)
	{
		_renderer = renderer;
	}

	asset_ref<texture> asset_provider_impl::load_texture(const char* path)
	{
        return static_cast<renderer_impl*>(_renderer)->load_texture(path);
	}

	asset_ref<material> asset_provider_impl::load_material(const char* path)
	{
        return static_cast<renderer_impl*>(_renderer)->load_material(path);
	}

	asset_ref<sound> asset_provider_impl::load_sound(const char* path)
	{
		return asset_ref<sound>();
	}

	asset_ref<prototype> asset_provider_impl::load_prototype(const char* path)
	{
		return asset_ref<prototype>();
	}
}