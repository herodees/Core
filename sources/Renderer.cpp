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

	void Renderer::init()
	{
	}

	void Renderer::deinit()
	{
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




	bool Renderer::begin_frame()
	{
		ray::BeginDrawing();
		return true;
	}

	void Renderer::end_frame()
	{
		ray::EndDrawing();
	}

	bool Renderer::begin_2d(const Camera& cam, bool depthsort)
	{
		_command.vertex_size = 0;
		_command.vertex = 0;
		_command.blendmode = ray::RL_BLEND_ALPHA;
		_command.texture = 0;
		_depth = 0;
		_depthsort = depthsort;
		_camera = cam;

		_verts.resize(0xffff);
		_depths.clear();
		_commands.clear();

		ray::Camera2D raycam;
		raycam.offset.x = _camera.offset.x;
		raycam.offset.y = _camera.offset.y;
		raycam.target.x = _camera.target.x;
		raycam.target.y = _camera.target.y;
		raycam.zoom = _camera.zoom;
		raycam.rotation = _camera.rotation;
		BeginMode2D(raycam);

		return true;
	}

	void Renderer::end_2d()
	{
		newCommand();

		if (_depthsort)
		{
			std::sort(std::execution::seq,
				_depths.begin(),
				_depths.end());
		}

		drawBuffers(_depths, _commands, _verts.data());

		ray::EndMode2D();

		_verts.resize(0xffff);
		_depths.clear();
		_commands.clear();
		_command.vertex_size = 0;
		_command.vertex = 0;
		_depth = 0;
	}

	bool Renderer::begin_scissor_2d(const Recti& rc)
	{
		if (rc.width() <= 0 || rc.height() <= 0)
		{
			_scissor_active = false;
			return false;
		}
		_scissor_active = true;

		if(_scissor == rc)
			return true;

		_scissor = rc;
		ray::BeginScissorMode(rc.min.x, rc.min.y, rc.width(), rc.height());

		return true;
	}

	void Renderer::end_scissor_2d()
	{
		if (!_scissor_active)
			return;
	}

	void Renderer::set_scissor(const Rect<int16_t>& rc)
	{
		if (_command.scissor == rc)
			return;
		newCommand();
		_command.scissor = rc;
	}

	void Renderer::set_shader(uint32_t shader)
	{
		if (_command.shader == shader)
			return;
		newCommand();
		_command.shader = shader;
	}

	uint32_t Renderer::get_shader() const
	{
		return _command.shader;
	}

	void Renderer::newCommand()
	{
		if (_command.vertex_size)
		{
			_depths.emplace_back(_depth, _commands.size());
			_commands.emplace_back(_command);
			_command.vertex += _command.vertex_size;
			_command.vertex_size = 0;
			_depth = 0;
		}
	}

	void Renderer::set_depth(uint32_t depth)
	{
		if (!_depthsort || _depth == depth)
			return;
		newCommand();
		_depth = depth;
	}


	uint32_t Renderer::get_depth() const
	{
		return _depth;
	}

	void Renderer::set_texture(uint32_t tx)
	{
		if (_command.texture == tx)
			return;
		newCommand();
		_command.texture = tx;
	}

	uint32_t Renderer::get_texture() const
	{
		return _command.texture;
	}

	void Renderer::set_blend_mode(uint32_t bm)
	{
		if (_command.blendmode == bm)
			return;
		newCommand();
		_command.blendmode = bm;
	}

	uint32_t  Renderer::get_blend_mode() const
	{
		return _command.blendmode;
	}

	Mesh Renderer::begin_mesh(uint32_t vtx)
	{
		Mesh msh;
		msh.vertex = &_verts[_command.vertex + _command.vertex_size];
		return msh;
	}

	void Renderer::end_mesh(const Mesh& m)
	{
		_command.vertex_size += m.vertex_size;
	}

	void Renderer::drawBuffers(const std::vector<std::pair<uint32_t, uint32_t>>& depths,
		const std::vector<Command>& cmds,
		const Vertex* vtx)
	{
		if (depths.size())
		{
			ray::rlBegin(RL_TRIANGLES);
			for (auto& ind : depths)
			{
				auto& command = cmds[ind.second];
				{
					if (ray::rlCheckRenderBatchLimit(command.vertex_size))
					{
						ray::rlBegin(RL_TRIANGLES);
					}

					ray::rlSetTexture(_textures[command.texture].id);
					ray::rlSetBlendMode(command.blendmode);

					for (auto i = 0; i < command.vertex_size; ++i)
					{
						auto& vertex = vtx[command.vertex + i];

						ray::rlColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
						ray::rlTexCoord2f(vertex.tex_coord.x, vertex.tex_coord.y);
						ray::rlVertex2f(vertex.position.x, vertex.position.y);
					}
				}
			}
			ray::rlEnd();
		}
		else
		{
			ray::rlBegin(RL_TRIANGLES);
			for (auto& command : cmds)
			{
				if (ray::rlCheckRenderBatchLimit(command.vertex_size))
				{
					ray::rlBegin(RL_TRIANGLES);
				}

				ray::rlSetTexture(_textures[command.texture].id);
				ray::rlSetBlendMode(command.blendmode);

				for (auto i = 0; i < command.vertex_size; ++i)
				{
					auto& vertex = vtx[command.vertex + i];

					ray::rlColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
					ray::rlTexCoord2f(vertex.tex_coord.x, vertex.tex_coord.y);
					ray::rlVertex2f(vertex.position.x, vertex.position.y);
				}
			}
			ray::rlEnd();
		}
	}
}