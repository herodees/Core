#pragma once

#include "Include.hpp"

namespace box
{
	class Renderer final : public IRenderer
	{
	public:
		Renderer();
		~Renderer() override;

		void begin_drawing() override;
		void end_drawing() override;
		void begin_texture_drawing(uint32_t id) override;
		void end_texture_drawing() override;
		void begin_2d_mode(Camera camera) override;
		void end_2d_mode() override;
		void begin_shader_mode(uint32_t id) override;
		void end_shader_mode() override;
		void begin_blend_mode(uint32_t mode) override;
		void end_blend_mode() override;
		void begin_scissor_mode(const Recti& rc) override;
		void end_scissor_mode() override;

		void clear_background(Color c) override;

		uint32_t load_texture(const char* path) override;
		void unload_texture(uint32_t id) override;
		Vec2i get_texture_size(uint32_t id) const override;
		void set_texture_filter(uint32_t id, uint32_t filter) override;
		void set_texture_wrap(uint32_t id, uint32_t wrap) override;
		void gen_texture_mipmap(uint32_t id) override;
		uint32_t load_image(const char* path) override;
		void unload_image(uint32_t id) override;
		uint32_t load_render_texture(int32_t w, int32_t h) override;
		void unload_render_texture(uint32_t id) override;

	private:
		std::vector<ray::Texture2D> _textures;
		std::vector<ray::RenderTexture2D> _render_textures;
		std::vector<ray::Image> _images;
		std::vector<ray::Shader> _shaders;
		uint32_t _free_texture = -1;
		uint32_t _free_render_texture = -1;
		uint32_t _free_image = -1;
		uint32_t _free_shader = -1;
	};


}