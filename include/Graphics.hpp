#pragma once

namespace box
{
	class IRenderer;
	class ITexture;
	class IMaterial;

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

	class IRenderTexture
	{
	public:
		virtual ~IRenderTexture() = default;
		virtual const ITexture& get_texture() const = 0;
		virtual const ITexture& get_depth() const = 0;
		virtual bool create(uint32_t width, uint32_t height, bool depth = false) = 0;
		const Vec2i& size() const { return _size; }

	protected:
		uint32_t _id{};
		Vec2i _size;
	};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;

		virtual void clear_background(Color c) = 0;

		virtual bool begin_2d(const Camera& cam, bool depthsort) = 0;
		virtual void end_2d() = 0;

		virtual void enable_scissor_test(const Recti& scissor) = 0;
		virtual void enable_render_texture(const IRenderTexture* rt) = 0;

		virtual void set_texture(const ITexture* texture) = 0;
		virtual void set_depth(uint32_t depth) = 0;
		virtual void set_material(IMaterial* material) = 0;
		virtual IMaterial* get_default_material() = 0;
		virtual IMaterial* get_material() = 0;
		virtual Mesh begin_mesh(uint32_t vertex) = 0;
		virtual void end_mesh(const Mesh& mesh) = 0;

		virtual IRenderTexture* load_render_texture(uint32_t width, uint32_t height, bool depth = false) = 0;
	};

}