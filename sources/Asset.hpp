#pragma once

#include "Include.hpp"

namespace box
{
	class renderer_impl;

	class asset_provider_impl final : public asset_provider
	{
	public:
		asset_provider_impl();
		~asset_provider_impl() override;

		void init(renderer* renderer);

		asset_ref<texture> load_texture(const char* path) override;
		asset_ref<material> load_material(const char* path) override;
		asset_ref<sound> load_sound(const char* path) override;
		asset_ref<prototype> load_prototype(const char* path) override;

	private:
		renderer* _renderer{};
	};
}