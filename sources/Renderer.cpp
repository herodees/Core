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
		: _default(this)
	{
	}

	Renderer::~Renderer()
	{
	}

	void Renderer::init()
	{
		ray::Shader& shader = _shaders.emplace_back();
		shader.id = ray::rlGetShaderIdDefault();
		shader.locs = ray::rlGetShaderLocsDefault();
		ray::Texture2D& texture = _textures.emplace_back();
	}

	void Renderer::deinit()
	{
		_shaders[0] = {};
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

	bool Renderer::begin_2d(const Camera& cam, bool depthsort, const Recti* scissor)
	{
		_command.vertex_size = 0;
		_command.vertex = 0;
		_command.blendmode = BlendMode::ALPHA;
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

		if (scissor)
			ray::BeginScissorMode(scissor->min.x, scissor->min.y, scissor->width(), scissor->height());

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

		ray::EndScissorMode();
		ray::EndMode2D();

		_verts.resize(0xffff);
		_depths.clear();
		_commands.clear();
		_command.vertex_size = 0;
		_command.vertex = 0;
		_depth = 0;
	}

	void Renderer::post_command(IRenderCommand* command)
	{
		if (_command.cmd == command)
			return;
		newCommand();
		_command.cmd = command;
	}

	void Renderer::post_depth(uint32_t depth)
	{
		if (!_depthsort || _depth == depth)
			return;
		newCommand();
		_depth = depth;
	}

	void Renderer::post_blend_mode(BlendMode blend)
	{
		if (_command.blendmode == blend)
			return;
		newCommand();
		_command.blendmode = blend;
	}

	void Renderer::post_texture(uint32_t texture)
	{
		if (_command.texture == texture)
			return;
		newCommand();
		_command.texture = texture;
	}


	void Renderer::newCommand()
	{
		if (_command.vertex_size || _command.cmd)
		{
			_depths.emplace_back(_depth, _commands.size());
			_commands.emplace_back(_command);
			_command.vertex += _command.vertex_size;
			_command.vertex_size = 0;
			_depth = 0;
		}
	}

	Mesh Renderer::begin_post_mesh(uint32_t vtx)
	{
		Mesh msh;
		msh.vertex = &_verts[_command.vertex + _command.vertex_size];
		return msh;
	}

	void Renderer::end_post_mesh(const Mesh& m)
	{
		_command.vertex_size += m.vertex_size;
	}

	void Renderer::set_scissor(const Recti* rc)
	{
		if (rc)
			ray::BeginScissorMode(rc->min.x, rc->min.y, rc->width(), rc->height());
		else
			ray::EndScissorMode();
	}

	void Renderer::set_texture(uint32_t texture)
	{
		ray::rlSetTexture(_textures[texture].id);
	}

	void Renderer::set_blend_mode(BlendMode blend)
	{
		ray::rlSetBlendMode((int32_t)blend);
	}

	void Renderer::set_uniform(uint32_t loc, const void* data, uint32_t type, uint32_t size)
	{
		ray::rlSetUniform(loc, data, type, size);
	}

	void Renderer::set_uniform_sampler(uint32_t loc, uint32_t texture)
	{
		ray::rlSetUniformSampler(loc, _textures[texture].id);
	}

	uint32_t Renderer::get_uniform_location(uint32_t shader, const char* name) const
	{
		return ray::rlGetLocationUniform(shader, name);
	}

	void Renderer::begin_set_shader(uint32_t shader)
	{
		ray::BeginShaderMode(_shaders[shader]);
	}

	void Renderer::end_set_shader()
	{
		ray::EndShaderMode();
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
					ray::rlSetBlendMode((int32_t)command.blendmode);

					if (command.cmd)
						command.cmd->call(*this);

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
				ray::rlSetBlendMode((int32_t)command.blendmode);

				if (command.cmd)
					command.cmd->call(*this);

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

	Material::Material(Renderer* r)
		: _renderer{r},
		_shader{ ray::rlGetShaderIdDefault(), ray::rlGetShaderLocsDefault() }
	{
	}

	bool Material::save(const char* path)
	{
		return false;
	}

	bool Material::load(const char* path)
	{
		return false;
	}

	void Material::bind(bool activate)
	{
		if(activate)
			ray::rlSetShader(_shader.id, _shader.locs);
		else
			ray::rlSetShader(ray::rlGetShaderIdDefault(), ray::rlGetShaderLocsDefault());
	}

	void Material::draw(const Vertex* vtx, size_t size)
	{
		if (ray::rlCheckRenderBatchLimit(size))
		{
			ray::rlBegin(RL_TRIANGLES);
		}

		ray::rlSetTexture(_texture[0]);
		ray::rlSetBlendMode((int32_t)_blend_mode);

		for (size_t i = 0; i < size; ++i)
		{
			auto& vertex = vtx[i];
			ray::rlColor4ub(vertex.color.r, vertex.color.g, vertex.color.b, vertex.color.a);
			ray::rlTexCoord2f(vertex.tex_coord.x, vertex.tex_coord.y);
			ray::rlVertex2f(vertex.position.x, vertex.position.y);
		}
	}

	void Material::set_shader(const char* vs, const char* fs)
	{
		if (_shader.id)
			ray::UnloadShader(_shader);
		_shader = ray::LoadShaderFromMemory(vs, fs);
	}

	void Material::set_blend_mode(BlendMode blend)
	{
		_blend_mode = blend;
	}

	void Material::set_texture(uint32_t loc, uint32_t texture)
	{
		for (size_t n = 0; n < std::size(_texture_loc); ++n)
		{
			if (!_texture_loc[n] || _texture_loc[n] == loc)
			{
				_texture_loc[n] = loc;
				_texture[n] = texture;
				break;
			}
		}
	}

	void Material::set_uniform(uint32_t loc, const void* data, uint32_t count, uint32_t type)
	{


	}

	uint32_t Material::get_location(const char* name) const
	{
		return ray::rlGetLocationAttrib(_shader.id, name);
	}
}