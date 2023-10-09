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
		void set_texture(uint32_t loc, uint32_t texture) override;
		void set_uniform(uint32_t loc, const void* data, uint32_t count, UniformType type) override;
		uint32_t get_location(const char* name) const override;

	protected:
		BlendMode _blend_mode{ BlendMode::ALPHA };
		ray::Shader _shader{};
		uint32_t _texture[4]{};
		uint32_t _texture_loc[4]{};
		Renderer* _renderer{};
	};

	class Texture : public ITexture
	{
		friend class Renderer;
	public:
		Texture(Renderer* r);
		~Texture() override;
		void set_filter(uint32_t filter) override;
		void set_wrap(uint32_t wrap) override;
		void generate_mipmap() override;
		bool save(const char* path) override;
		bool load(const char* path) override;
	protected:
		Renderer* _renderer;
	};


	class Renderer final : public IRenderer
	{
		struct Command
		{
			uint32_t depth;
			uint32_t vertex;
			uint32_t vertex_size;
			uint32_t texture;
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
		void enable_render_texture(uint32_t rt) override;

		void set_texture(uint32_t texture) override;
		void set_depth(uint32_t depth) override;
		void set_material(IMaterial* material) override;
		IMaterial* get_default_material() override;
		IMaterial* get_material() override;
		Mesh begin_mesh(uint32_t vertex) override;
		void end_mesh(const Mesh& mesh) override;

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

		Texture* load_texture_asset(const char* path);
		Material* load_material_asset(const char* path);

	protected:
		void newCommand();

	private:
		std::vector<Vertex>  _verts;
		std::vector<Command> _commands;
		std::vector<ray::Texture2D> _textures;
		std::vector<ray::RenderTexture2D> _render_textures;
		std::vector<ray::Image> _images;
		uint32_t _free_texture = -1;
		uint32_t _free_render_texture = -1;
		uint32_t _free_image = -1;
		uint32_t _free_shader = -1;
		uint32_t _free_material = -1;
		Command _command{};
		bool _depthsort{};
		Camera _camera{};
		Material _default;
	};




}