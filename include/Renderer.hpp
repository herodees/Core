#pragma once

namespace box
{
	class IRenderCommand;
	class IRenderer;

	enum class BlendMode
	{
		ALPHA = 0,                 // Blend textures considering alpha (default)
		ADDITIVE,                  // Blend textures adding colors
		MULTIPLIED,                // Blend textures multiplying colors
		ADD_COLORS,                // Blend textures adding colors (alternative)
		SUBTRACT_COLORS,           // Blend textures subtracting colors (alternative)
		ALPHA_PREMULTIPLY,         // Blend premultiplied textures considering alpha
		CUSTOM,                    // Blend textures using custom src/dst factors (use rlSetBlendFactors())
		CUSTOM_SEPARATE            // Blend textures using custom src/dst factors (use rlSetBlendFactorsSeparate())
	};

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

	struct Vertex
	{
		Vec2f position;
		Color color;
		Vec2f tex_coord;
	};

	struct Mesh
	{
		Vertex* vertex{};
		uint32_t vertex_size{};
	};

	class IMaterial
	{
	public:
		uint32_t id{};

		virtual ~IMaterial() = default;

		virtual bool save(const char* path) = 0;
		virtual bool load(const char* path) = 0;
		virtual void bind(bool activate) = 0;
		virtual void draw(const Vertex* vtx, size_t size) = 0;
		virtual void set_shader(const char* vs, const char* fs) = 0;
		virtual void set_blend_mode(BlendMode blend) = 0;
		virtual void set_texture(uint32_t loc, uint32_t texture) = 0;
		virtual void set_uniform(uint32_t loc, const void* data, uint32_t count, uint32_t type) = 0;
		virtual uint32_t get_location(const char* name) const = 0;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void clear_background(Color c) = 0;

		virtual void post_command(IRenderCommand* command) = 0;
		virtual void post_depth(uint32_t depth) = 0;
		virtual void post_blend_mode(BlendMode blend) = 0;
		virtual void post_texture(uint32_t texture) = 0;
		virtual Mesh begin_post_mesh(uint32_t vertex) = 0;
		virtual void end_post_mesh(const Mesh& m) = 0;

		virtual void set_scissor(const Recti* rc) = 0;
		virtual void set_texture(uint32_t texture) = 0;
		virtual void set_blend_mode(BlendMode blend) = 0;
		virtual void begin_set_shader(uint32_t shader) = 0;
		virtual void end_set_shader() = 0;
		virtual void set_uniform(uint32_t loc, const void* data, uint32_t type, uint32_t size) = 0;
		virtual void set_uniform_sampler(uint32_t loc, uint32_t texture) = 0;
		virtual uint32_t get_uniform_location(uint32_t shader, const char* name) const = 0;

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
		virtual uint32_t load_shader(const char* vs_path, const char* fs_path) = 0;
		virtual void unload_shader(uint32_t id) = 0;
	};

	class IRenderCommand
	{
	public:
		virtual ~IRenderCommand() = default;
		virtual void call(IRenderer& renderer) = 0;
	};

}