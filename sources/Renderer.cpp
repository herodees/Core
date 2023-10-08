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
		ray::Texture2D& texture = _textures.emplace_back();
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

	Texture* Renderer::load_texture_asset(const char* path)
	{
		auto txt = ray::LoadTexture(path);
		if (!txt.id)
			return nullptr;
		return new Texture(this, txt);
	}

	Material* Renderer::load_material_asset(const char* path)
	{
		return nullptr;
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




	bool Renderer::begin_2d(const Camera& cam, bool depthsort)
	{
		_command.vertex_size = 0;
		_command.vertex = 0;
		_command.texture = 0;
		_command.material = &_default;
		_depthsort = depthsort;
		_camera = cam;

		_verts.resize(0xffff);
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
				_commands.begin(),
				_commands.end(),
				[](const Command& a, const Command& b)
				{
					return a.depth < b.depth;
				});
		}

		ray::rlBegin(RL_TRIANGLES);
		for (auto& cmd : _commands)
		{
			ray::rlSetTexture(_textures[cmd.texture].id);
			cmd.material->draw(_verts.data() + cmd.vertex, cmd.vertex_size);
		}
		ray::rlEnd();

		ray::EndScissorMode();
		ray::EndTextureMode();
		ray::EndMode2D();

		_verts.resize(0xffff);
		_commands.clear();
		_command.vertex_size = 0;
		_command.vertex = 0;
		_command.depth = 0;
		_command.material = &_default;
	}

	void Renderer::enable_scissor_test(const Recti& scissor)
	{
		ray::BeginScissorMode(scissor.min.x, scissor.min.y, scissor.width(), scissor.height());
	}

	void Renderer::enable_render_texture(uint32_t rt)
	{
		ray::BeginTextureMode(_render_textures[rt]);
	}

	void Renderer::set_material(IMaterial* material)
	{
		if (!_depthsort)
		{
			_command.material = material;
		}
		else if (_command.material != material)
		{
			newCommand();
			_command.material = material;
		}
	}

	void Renderer::set_texture(uint32_t texture)
	{
		if (!_depthsort)
		{
			ray::rlSetTexture(_textures[texture].id);
			_command.texture = texture;
		}
		else if (_command.texture != texture)
		{
			newCommand();
			_command.texture = texture;
		}
	}

	void Renderer::set_depth(uint32_t depth)
	{
		if (!_depthsort)
		{
		}
		else if (_command.depth != depth)
		{
			newCommand();
			_command.depth = depth;
		}
	}

	IMaterial* Renderer::get_default_material()
	{
		return &_default;
	}

	IMaterial* Renderer::get_material()
	{
		return _command.material;
	}

	Mesh Renderer::begin_mesh(uint32_t vertex)
	{
		return Mesh{ &_verts[_command.vertex + _command.vertex_size], 0 };
	}

	void Renderer::end_mesh(const Mesh& mesh)
	{
		if (_depthsort)
		{
			_command.vertex_size += mesh.vertex_size;
		}
		else
		{
			ray::rlBegin(RL_TRIANGLES);
			ray::rlSetTexture(_textures[_command.texture].id);
			_command.material->draw(mesh.vertex, mesh.vertex_size);
			ray::rlEnd();
		}
	}

	void Renderer::newCommand()
	{
		if (_command.vertex_size)
		{
			_commands.emplace_back(_command);
			_command.vertex += _command.vertex_size;
			_command.vertex_size = 0;
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

	void Material::set_uniform(uint32_t loc, const void* data, uint32_t count, UniformType type)
	{
		ray::rlSetUniform(loc, data, (int32_t)type, count);
	}

	uint32_t Material::get_location(const char* name) const
	{
		return ray::rlGetLocationAttrib(_shader.id, name);
	}

	Texture::Texture(Renderer* r, ray::Texture txt)
		: _renderer(r)
	{
		_id = txt.id;
		_size.x = txt.width;
		_size.y = txt.height;
		_mipmaps = txt.mipmaps;
		_format = txt.format;
	}

	Texture::~Texture()
	{
		if(_id)
			ray::rlUnloadTexture(_id);
	}

	void Texture::set_filter(uint32_t filter)
	{
		ray::SetTextureFilter({ _id }, filter);
	}

	void Texture::set_wrap(uint32_t wrap)
	{
		ray::SetTextureWrap({ _id }, wrap);
	}

	void Texture::generate_mipmap()
	{
		ray::Texture txt;
		txt.id = _id;
		txt.width = _size.x;
		txt.height = _size.y;
		txt.mipmaps = _mipmaps;
		txt.format = _format;
		ray::GenTextureMipmaps(&txt);
		_mipmaps = txt.mipmaps;
	}

	bool Texture::save(const char* path)
	{
	

		return false;
	}

	bool Texture::load(const char* path)
	{
		return false;
	}
}