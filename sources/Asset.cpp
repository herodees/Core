#include "Asset.hpp"
#include "Renderer.hpp"

namespace box
{
    template <typename ASSET, typename BASE>
    asset_ref<BASE> asset_provider_impl::load_asset(const char* path)
    {
        auto ret = new ASSET();
        ret->set_asset_provider(this);
        ret->load(path);
        return ret;
    }

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
        return load_asset<texture_impl, texture>(path);
	}

	asset_ref<material> asset_provider_impl::load_material(const char* path)
    {
        return load_asset<material_impl, material>(path);
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