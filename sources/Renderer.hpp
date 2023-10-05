#pragma once

#include "Include.hpp"

namespace box
{
	class Renderer final : public IRenderer
	{
		struct Command
		{
			uint32_t vertex;
			uint32_t vertex_size;
			Rect<int16_t> scissor;
			uint16_t blendmode;
			uint16_t texture;
			uint16_t shader;
		};
	public:
		Renderer();
		~Renderer() override;

		void init();
		void deinit();

		bool begin_frame();
		void end_frame();

		bool begin_2d(const Camera& cam, bool depthsort);
		void end_2d();

		bool begin_scissor_2d(const Recti& rc);
		void end_scissor_2d();

		void set_scissor(const Rect<int16_t>& rc);
		void set_shader(uint32_t shader);
		uint32_t get_shader() const;
		void set_depth(uint32_t depth);
		uint32_t get_depth() const;
		void set_blend_mode(uint32_t bm);
		uint32_t get_blend_mode() const;
		void set_texture(uint32_t tx);
		uint32_t get_texture() const;
		Mesh begin_mesh(uint32_t vtx);
		void end_mesh(const Mesh& m);


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
		uint32_t load_shader(const char* vs_path, const char* fs_path) override;
		void unload_shader(uint32_t id) override;

	protected:
		void newCommand();
		void drawBuffers(const std::vector<std::pair<uint32_t, uint32_t>>& depths, const std::vector<Command>& cmds, const Vertex* vtx);

	private:
		std::vector<ray::Texture2D> _textures;
		std::vector<ray::RenderTexture2D> _render_textures;
		std::vector<ray::Image> _images;
		std::vector<ray::Shader> _shaders;
		uint32_t _free_texture = -1;
		uint32_t _free_render_texture = -1;
		uint32_t _free_image = -1;
		uint32_t _free_shader = -1;
		Command _command{};
		std::vector<Vertex>  _verts;
		std::vector<Command> _commands;
		std::vector<std::pair<uint32_t, uint32_t>> _depths;
		bool _depthsort{};
		Camera _camera{};
		uint32_t _depth{};
		Recti _scissor{};
		bool _scissor_active{};
	};


}