#pragma once

#include "Include.hpp"

namespace box
{
	class renderer_impl;

	class material_impl final : public material
	{
	public:
		material_impl(renderer_impl* r);
		~material_impl() override = default;

		bool save(const char* path) override;
		bool load(const char* path) override;
		void bind(bool activate) override;
		void draw(const vertex* vtx, size_t size) override;
		void set_shader(const char* vs, const char* fs) override;
		void set_blend_mode(blend_mode blend) override;
		void set_texture(uint32_t loc, const texture* texture) override;
		void set_uniform(uint32_t loc, const void* data, uint32_t count, uniform_type type) override;
		uint32_t get_location(const char* name) const override;
		bool compile();

	protected:
		renderer_impl* _renderer{};
		blend_mode _blend_mode{ blend_mode::ALPHA };
		ray::Shader _shader{};
		std::string _fragment;
		std::string _vertex;
	};



	class texture_impl final : public texture
	{
		friend class renderer_impl;
	public:
		texture_impl(renderer_impl* r);
		~texture_impl() override;
		void attach(const ray::Texture& txt);
		void set_filter(uint32_t filter) override;
		void set_wrap(uint32_t wrap) override;
		void generate_mipmap() override;
		bool save(const char* path) override;
		bool load(const char* path) override;

		ray::Texture get() const;

	protected:
		renderer_impl* _renderer;
	};



	class render_texture_impl final : public render_texture
	{
	public:
		render_texture_impl(renderer_impl* r);
		~render_texture_impl() override;
		const texture& get_texture() const override;
		const texture& get_depth() const override;
		bool create(uint32_t width, uint32_t height, bool depth = false) override;

		ray::RenderTexture get() const;

	protected:
		texture_impl _texture;
		texture_impl _depth;
	};



	class renderer_impl final : public renderer
	{
		struct command
		{
			uint32_t depth;
			uint32_t vertex;
			uint32_t vertex_size;
			const texture* texture;
			material* material;
		};
	public:
		renderer_impl();
		~renderer_impl() override;

		void init();
		void deinit();

		void update(scene& scn, float delta) override;

		bool begin_2d(const camera& cam, bool depthsort) override;
		void end_2d() override;

		void enable_scissor_test(const Recti& scissor) override;
		void enable_render_texture(const render_texture* rt) override;

		void set_texture(const texture* texture) override;
		void set_depth(uint32_t depth) override;
		void set_material(material* material) override;
		material* get_default_material() override;
		material* get_material() override;
		mesh begin_mesh(uint32_t vertex) override;
		void end_mesh(const mesh& mesh) override;

		void draw_line(Vec2f p1, Vec2f p2, color clr) override;
        void draw_polyline(const Vec2f* p, size_t size, bool closed, color clr) override;
        void draw_circle_segment(Vec2f c, float r, color clr, uint32_t segments, float start_ang = 0.f, float end_ang = 360.f) override;
        void draw_rectangle(Rectf rc, color clr) override;
        void draw_ellipse(Rectf rc, color clr) override;

		void clear_background(color c) override;

		texture_impl* load_texture(const char* path);
		material_impl* load_material(const char* path);
		render_texture* load_render_texture(uint32_t width, uint32_t height, bool depth = false) override;

	protected:
		void new_command();

	private:
		std::vector<vertex>  _verts;
		std::vector<command> _commands;
		command _command{};
		bool _depthsort{};
		camera _camera{};
		const render_texture* _target{};
		material_impl _default;
	};

}