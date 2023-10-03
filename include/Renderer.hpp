#pragma once

namespace box
{
	struct Color
	{
		uint8_t r;
		uint8_t g;
		uint8_t b;
		uint8_t a;
	};

	struct Camera
	{
		Vec2f offset;
		Vec2f target;
		float rotation{0.f};
		float zoom{1.f};
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void begin_drawing() = 0;
		virtual void end_drawing() = 0;
		virtual void begin_texture_drawing(uint32_t id) = 0;
		virtual void end_texture_drawing() = 0;
		virtual void begin_2d_mode(Camera camera) = 0;
		virtual void end_2d_mode() = 0;
		virtual void begin_shader_mode(uint32_t id) = 0;
		virtual void end_shader_mode() = 0;
		virtual void begin_blend_mode(uint32_t mode) = 0;
		virtual void end_blend_mode() = 0;
		virtual void begin_scissor_mode(const Recti& rc) = 0;
		virtual void end_scissor_mode() = 0;

		virtual void clear_background(Color c) = 0;

		virtual uint32_t load_texture(const char* path) = 0;
		virtual void unload_texture(uint32_t id) = 0;
		virtual Vec2i get_texture_size(uint32_t id) const = 0;
		virtual void set_texture_filter(uint32_t id, uint32_t filter) = 0;
		virtual void set_texture_wrap(uint32_t id, uint32_t wrap) = 0;
		virtual void gen_texture_mipmap(uint32_t id) = 0;
		virtual uint32_t load_image(const char* path) = 0;
		virtual void unload_image(uint32_t id) = 0;
		virtual uint32_t load_render_texture(int32_t w, int32_t h) = 0;
		virtual void unload_render_texture(uint32_t id) = 0;
	};

}