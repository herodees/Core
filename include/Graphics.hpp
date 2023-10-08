#pragma once

namespace box
{
	class IRenderer;

	enum class BlendMode
	{
		ALPHA = 0,
		ADDITIVE,
		MULTIPLIED,
		ADD_COLORS,
		SUBTRACT_COLORS,
		ALPHA_PREMULTIPLY,
		CUSTOM,
		CUSTOM_SEPARATE
	};

	enum class UniformType
	{
		FLOAT = 0,
		VEC2,
		VEC3,
		VEC4,
		INT,
		IVEC2,
		IVEC3,
		IVEC4,
		SAMPLER2D
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

	class IMaterial : public IAsset
	{
	public:
		virtual ~IMaterial() = default;
		virtual void bind(bool activate) = 0;
		virtual void draw(const Vertex* vtx, size_t size) = 0;
		virtual void set_shader(const char* vs, const char* fs) = 0;
		virtual void set_blend_mode(BlendMode blend) = 0;
		virtual void set_texture(uint32_t loc, uint32_t texture) = 0;
		virtual void set_uniform(uint32_t loc, const void* data, uint32_t count, UniformType type) = 0;
		virtual uint32_t get_location(const char* name) const = 0;
	};

	class ITexture : public IAsset
	{
	public:
		virtual ~ITexture() = default;
		const Vec2i& size() const { return _size; }
		uint32_t handle() const { return _id; }
		int32_t get_format() const { return _format; }
		virtual void set_filter(uint32_t filter) = 0;
		virtual void set_wrap(uint32_t wrap) = 0;
		virtual void generate_mipmap() = 0;

	protected:
		uint32_t _id{};
		Vec2i _size{};
		int32_t _mipmaps{};
		int32_t _format{};
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void clear_background(Color c) = 0;

		virtual bool begin_2d(const Camera& cam, bool depthsort) = 0;
		virtual void end_2d() = 0;

		virtual void enable_scissor_test(const Recti& scissor) = 0;
		virtual void enable_render_texture(uint32_t rt) = 0;

		virtual void set_texture(uint32_t texture) = 0;
		virtual void set_depth(uint32_t depth) = 0;
		virtual void set_material(IMaterial* material) = 0;
		virtual IMaterial* get_default_material() = 0;
		virtual IMaterial* get_material() = 0;
		virtual Mesh begin_mesh(uint32_t vertex) = 0;
		virtual void end_mesh(const Mesh& mesh) = 0;

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