#pragma once

#include "Include.hpp"

namespace box
{
	class Renderer;

	class Material : public IMaterial
	{
	public:
		Material(Renderer* r);
		~Material() override = default;

		bool save(const char* path) override;
		bool load(const char* path) override;
		void bind(bool activate) override;
		void draw(const Vertex* vtx, size_t size) override;
		void set_shader(const char* vs, const char* fs) override;
		void set_blend_mode(BlendMode blend) override;
		void set_texture(uint32_t loc, const ITexture* texture) override;
		void set_uniform(uint32_t loc, const void* data, uint32_t count, UniformType type) override;
		uint32_t get_location(const char* name) const override;
		bool compile();

	protected:
		Renderer* _renderer{};
		BlendMode _blend_mode{ BlendMode::ALPHA };
		ray::Shader _shader{};
		std::string _fragment;
		std::string _vertex;
	};


	class Texture : public ITexture
	{
		friend class Renderer;
	public:
		Texture(Renderer* r);
		~Texture() override;
		void attach(const ray::Texture& txt);
		void set_filter(uint32_t filter) override;
		void set_wrap(uint32_t wrap) override;
		void generate_mipmap() override;
		bool save(const char* path) override;
		bool load(const char* path) override;

		ray::Texture get() const;

	protected:
		Renderer* _renderer;
	};



	class RenderTexture : public IRenderTexture
	{
	public:
		RenderTexture(Renderer* r);
		~RenderTexture() override;
		const ITexture& get_texture() const override;
		const ITexture& get_depth() const override;
		bool create(uint32_t width, uint32_t height, bool depth = false) override;

		ray::RenderTexture get() const;

	protected:
		Texture _texture;
		Texture _depth;
	};



	class Renderer final : public IRenderer
	{
		struct Command
		{
			uint32_t depth;
			uint32_t vertex;
			uint32_t vertex_size;
			const ITexture* texture;
			IMaterial* material;
		};
	public:
		Renderer();
		~Renderer() override;

		void init();
		void deinit();

		bool begin_2d(const Camera& cam, bool depthsort) override;
		void end_2d() override;

		void enable_scissor_test(const Recti& scissor) override;
		void enable_render_texture(const IRenderTexture* rt) override;

		void set_texture(const ITexture* texture) override;
		void set_depth(uint32_t depth) override;
		void set_material(IMaterial* material) override;
		IMaterial* get_default_material() override;
		IMaterial* get_material() override;
		Mesh begin_mesh(uint32_t vertex) override;
		void end_mesh(const Mesh& mesh) override;

		void clear_background(Color c) override;

		Texture* load_texture(const char* path);
		Material* load_material(const char* path);

		IRenderTexture* load_render_texture(uint32_t width, uint32_t height, bool depth = false) override;

	protected:
		void newCommand();

	private:
		std::vector<Vertex>  _verts;
		std::vector<Command> _commands;
		Command _command{};
		bool _depthsort{};
		Camera _camera{};
		const IRenderTexture* _target{};
		Material _default;
	};




}