#include "Renderer.hpp"
#include "Renderer.hpp"
#include "Renderer.hpp"

namespace box
{
	template <typename T>
	uint32_t create_index(T& data, uint32_t& free_id)
	{
		if (free_id < data.size())
		{
			const uint32_t ret = free_id;
			free_id = (uint32_t&)data[free_id];
			return ret;
		}
		data.emplace_back();
		return uint32_t(data.size() - 1);
	}

	template <typename T>
	void free_index(T& data, uint32_t id, uint32_t& free_id)
	{
		(uint32_t&)(data[id]) = free_id;
		free_id = id;
	}

	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::begin_drawing()
	{
		ray::BeginDrawing();
	}

	void Renderer::end_drawing()
	{
		ray::EndDrawing();
	}

	void Renderer::begin_texture_drawing(uint32_t id)
	{
		ray::BeginTextureMode(_render_textures[id]);
	}

	void Renderer::end_texture_drawing()
	{
		ray::EndTextureMode();
	}

	void Renderer::begin_2d_mode(Camera camera)
	{
		ray::BeginMode2D((ray::Camera2D&)camera);
	}

	void Renderer::end_2d_mode()
	{
		ray::EndMode2D();
	}

	void Renderer::begin_shader_mode(uint32_t id)
	{
		ray::BeginShaderMode(_shaders[id]);
	}

	void Renderer::end_shader_mode()
	{
		ray::EndShaderMode();
	}

	void Renderer::begin_blend_mode(uint32_t mode)
	{
		ray::BeginBlendMode(mode);
	}

	void Renderer::end_blend_mode()
	{
		ray::EndBlendMode();
	}

	void Renderer::begin_scissor_mode(const Recti& rc)
	{
		ray::BeginScissorMode(rc.min.x, rc.min.y, rc.max.x - rc.min.x, rc.max.y - rc.min.y);
	}

	void Renderer::end_scissor_mode()
	{
		ray::EndScissorMode();
	}

	void Renderer::clear_background(Color c)
	{
		ray::ClearBackground((ray::Color&)c);

		ray::Camera2D cam;
	}

	uint32_t Renderer::load_texture(const char* path)
	{
		auto ret = create_index(_textures, _free_texture);
		_textures[ret] = ray::LoadTexture(path);
		return ret;
	}

	void Renderer::unload_texture(uint32_t id)
	{
		ray::UnloadTexture(_textures[id]);
		free_index(_textures, id, _free_texture);
	}

	Vec2i Renderer::get_texture_size(uint32_t id) const
	{
		return Vec2i(_textures[id].width, _textures[id].height);
	}

	void Renderer::set_texture_filter(uint32_t id, uint32_t filter)
	{
		ray::SetTextureFilter(_textures[id], filter);
	}

	void Renderer::set_texture_wrap(uint32_t id, uint32_t wrap)
	{
		ray::SetTextureWrap(_textures[id], wrap);
	}

	void Renderer::gen_texture_mipmap(uint32_t id)
	{
		ray::GenTextureMipmaps(&_textures[id]);
	}

	
	uint32_t Renderer::load_render_texture(int32_t w, int32_t h)
	{
		auto ret = create_index(_render_textures, _free_render_texture);
		_render_textures[ret] = ray::LoadRenderTexture(w, h);
		return ret;
	}

	void Renderer::unload_render_texture(uint32_t id)
	{
		ray::UnloadRenderTexture(_render_textures[id]);
		free_index(_render_textures, id, _free_render_texture);
	}

	uint32_t Renderer::load_shader(const char* vs_path, const char* fs_path)
	{
		auto ret = create_index(_shaders, _free_shader);
		_shaders[ret] = ray::LoadShader(vs_path, fs_path);
		return ret;
	}

	void Renderer::unload_shader(uint32_t id)
	{
		ray::UnloadShader(_shaders[id]);
		free_index(_shaders, id, _free_shader);
	}

	uint32_t Renderer::load_image(const char* path)
	{
		auto ret = create_index(_images, _free_image);
		_images[ret] = ray::LoadImage(path);
		return ret;
	}

	void Renderer::unload_image(uint32_t id)
	{
		ray::UnloadImage(_images[id]);
		free_index(_images, id, _free_image);
	}



}