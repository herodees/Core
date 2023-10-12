#include "Renderer.hpp"
#include "Asset.hpp"

namespace box
{

	renderer_impl::renderer_impl()
		: _default(this)
	{
	}

	renderer_impl::~renderer_impl()
	{
	}

	void renderer_impl::init()
	{
	}

	void renderer_impl::deinit()
	{
	}

	void renderer_impl::clear_background(color c)
	{
		ray::ClearBackground((ray::Color&)c);

		ray::Camera2D cam;
	}

	texture_impl* renderer_impl::load_texture(const char* path)
	{
		auto ret = new texture_impl(this);
		ret->load(path);
		return ret;
	}

	material_impl* renderer_impl::load_material(const char* path)
	{
		auto ret = new material_impl(this);
		ret->load(path);
		return ret;
	}

	render_texture* renderer_impl::load_render_texture(uint32_t width, uint32_t height, bool depth)
	{
		auto ret = new render_texture_impl(this);
		ret->create(width, height, depth);
		return ret;
	}

	bool renderer_impl::begin_2d(const camera& cam, bool depthsort)
	{
		_command.vertex_size = 0;
		_command.vertex = 0;
		_command.texture = 0;
		_command.material = &_default;
		_depthsort = depthsort;
		_camera = cam;
		_target = nullptr;

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

	void renderer_impl::end_2d()
	{
		new_command();

		if (_depthsort)
		{
			std::sort(std::execution::seq,
				_commands.begin(),
				_commands.end(),
				[](const command& a, const command& b)
				{
					return a.depth < b.depth;
				});
		}

		ray::rlBegin(RL_TRIANGLES);
		for (auto& cmd : _commands)
		{
			ray::rlSetTexture(cmd.texture ? cmd.texture->handle() : 0);
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


	void renderer_impl::enable_scissor_test(const Recti& scissor)
	{
		ray::BeginScissorMode(scissor.min.x, scissor.min.y, scissor.width(), scissor.height());
	}

	void renderer_impl::enable_render_texture(const render_texture* rt)
	{
		if (_target == rt)
			return;

		_target = rt;

		if (_target)
		{
			ray::BeginTextureMode(static_cast<const render_texture_impl*>(_target)->get());
		}
		else
		{
			ray::EndTextureMode();
		}
	}

	void renderer_impl::set_material(material* material)
	{
		if (!_depthsort)
		{
			_command.material = material;
		}
		else if (_command.material != material)
		{
			new_command();
			_command.material = material;
		}
	}

	void renderer_impl::set_texture(const texture* texture)
	{
		if (!_depthsort)
		{
			ray::rlSetTexture(texture ? texture->handle() : 0);
			_command.texture = texture;
		}
		else if (_command.texture != texture)
		{
			new_command();
			_command.texture = texture;
		}
	}

	void renderer_impl::set_depth(uint32_t depth)
	{
		if (!_depthsort)
		{
		}
		else if (_command.depth != depth)
		{
			new_command();
			_command.depth = depth;
		}
	}

	material* renderer_impl::get_default_material()
	{
		return &_default;
	}

	material* renderer_impl::get_material()
	{
		return _command.material;
	}

	mesh renderer_impl::begin_mesh(uint32_t vertex)
	{
		return mesh{ &_verts[_command.vertex + _command.vertex_size], 0 };
	}

	void renderer_impl::end_mesh(const mesh& mesh)
	{
		if (_depthsort)
		{
			_command.vertex_size += mesh.vertex_size;
		}
		else
		{
			ray::rlBegin(RL_TRIANGLES);
			ray::rlSetTexture(_command.texture ? _command.texture->handle() : 0);
			_command.material->draw(mesh.vertex, mesh.vertex_size);
			ray::rlEnd();
		}
	}

	void renderer_impl::new_command()
	{
		if (_command.vertex_size)
		{
			_commands.emplace_back(_command);
			_command.vertex += _command.vertex_size;
			_command.vertex_size = 0;
		}
	}
	
	material_impl::material_impl(renderer_impl* r)
		: _renderer{r},
		_shader{ ray::rlGetShaderIdDefault(), ray::rlGetShaderLocsDefault() }
	{
	}

	bool material_impl::save(const char* path)
	{
		var doc;
		doc.set_item("fragment", std::string_view(_fragment));
		doc.set_item("vertex", std::string_view(_vertex));
		doc.set_item("blend", (int32_t)_blend_mode);
		std::string out;
		doc.to_string(out);

		return ray::SaveFileData(path, out.data(), out.size() + 1);;
	}

	bool material_impl::load(const char* path)
	{
		int32_t size{};
		auto* data = ray::LoadFileData(path, &size);
		if (!data)
			return false;

		var doc;
		auto ret = doc.from_string((const char*)data);
		ray::UnloadFileData(data);
        if (ret != var_error::ok)
			return false;

		_fragment.assign(doc.get_item("fragment").str());
		_vertex.assign(doc.get_item("vertex").str());
		_blend_mode = (blend_mode)doc.get_item("blend").get(0);

		return compile();
	}

	void material_impl::bind(bool activate)
	{
		if(activate)
			ray::rlSetShader(_shader.id, _shader.locs);
		else
			ray::rlSetShader(ray::rlGetShaderIdDefault(), ray::rlGetShaderLocsDefault());
	}

	void material_impl::draw(const vertex* vtx, size_t size)
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

	void material_impl::set_shader(const char* vs, const char* fs)
	{
		if (_shader.id)
			ray::UnloadShader(_shader);
		_shader = ray::LoadShaderFromMemory(vs, fs);
	}

	void material_impl::set_blend_mode(blend_mode blend)
	{
		_blend_mode = blend;
	}

	void material_impl::set_texture(uint32_t loc, const texture* texture)
	{

	}

	void material_impl::set_uniform(uint32_t loc, const void* data, uint32_t count, uniform_type type)
	{
		ray::rlSetUniform(loc, data, (int32_t)type, count);
	}

	uint32_t material_impl::get_location(const char* name) const
	{
		return ray::rlGetLocationAttrib(_shader.id, name);
	}

	bool material_impl::compile()
	{
		if (_shader.id)
		{
			ray::UnloadShader(_shader);
		}
		_shader = ray::LoadShaderFromMemory(_vertex.c_str(), _fragment.c_str());
		return !!_shader.id;
	}

	texture_impl::texture_impl(renderer_impl* r)
		: _renderer(r)
	{
	}

	texture_impl::~texture_impl()
	{
		if(_id)
			ray::rlUnloadTexture(_id);
	}

	void texture_impl::attach(const ray::Texture& txt)
	{
		if (_id)
			ray::rlUnloadTexture(_id);
		_id = txt.id;
		_size.x = txt.width;
		_size.y = txt.height;
		_mipmaps = txt.mipmaps;
		_format = txt.format;
	}

	void texture_impl::set_filter(uint32_t filter)
	{
		ray::SetTextureFilter({ _id }, filter);
	}

	void texture_impl::set_wrap(uint32_t wrap)
	{
		ray::SetTextureWrap({ _id }, wrap);
	}

	void texture_impl::generate_mipmap()
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

	bool texture_impl::save(const char* path)
	{
		return false;
	}

	bool texture_impl::load(const char* path)
	{
		std::string_view str(path);
		std::string_view ext = str.substr(str.rfind('.'));

		int32_t size{};
		auto* data = ray::LoadFileData(path, &size);
		if (!data)
			return false;

		ray::Image image = ray::LoadImageFromMemory(ext.data(), data, size);
		auto txt = ray::LoadTextureFromImage(image);
		ray::UnloadFileData(data);
		ray::UnloadImage(image);

		attach(txt);
		return !!_id;
	}

	ray::Texture texture_impl::get() const
	{
		ray::Texture out;
		out.id = _id;
		out.width = _size.x;
		out.height = _size.y;
		out.mipmaps = _mipmaps;
		out.format = _format;
		return out;
	}


	render_texture_impl::render_texture_impl(renderer_impl* r) :
		_texture(r), _depth(r)
	{
	}

	render_texture_impl::~render_texture_impl()
	{
	}

	const texture& render_texture_impl::get_texture() const
	{
		return _texture;
	}

	const texture& render_texture_impl::get_depth() const
	{
		return _depth;
	}

	ray::RenderTexture render_texture_impl::get() const
	{
		ray::RenderTexture out;
		out.id = _id;
		out.depth = _depth.get();
		out.texture = _texture.get();
		return out;
	}

	bool render_texture_impl::create(uint32_t width, uint32_t height, bool depth)
	{
		_id = ray::rlLoadFramebuffer(width, height);   // Load an empty framebuffer

		if (_id > 0)
		{
			ray::rlEnableFramebuffer(_id);

			ray::Texture texture{};

			// Create color texture (default to RGBA)
			texture.id = ray::rlLoadTexture(NULL, width, height, ray::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8, 1);
			texture.width = width;
			texture.height = height;
			texture.format = ray::PIXELFORMAT_UNCOMPRESSED_R8G8B8A8;
			texture.mipmaps = 1;
			_texture.attach(texture);

			if (depth)
			{
				// Create depth renderbuffer/texture
				texture.id = ray::rlLoadTextureDepth(width, height, true);
				texture.width = width;
				texture.height = height;
				texture.format = 19;       //DEPTH_COMPONENT_24BIT?
				texture.mipmaps = 1;
				_depth.attach(texture);
			}

			// Attach color texture and depth renderbuffer/texture to FBO
			ray::rlFramebufferAttach(_id, _texture.handle(), ray::RL_ATTACHMENT_COLOR_CHANNEL0, ray::RL_ATTACHMENT_TEXTURE2D, 0);
			if (depth)
				ray::rlFramebufferAttach(_id, _texture.handle(), ray::RL_ATTACHMENT_DEPTH, ray::RL_ATTACHMENT_RENDERBUFFER, 0);

			// Check if fbo is complete with attachments (valid)
			if (ray::rlFramebufferComplete(_id)) TRACELOG(LOG_INFO, "FBO: [ID %i] Framebuffer object created successfully", _id);

			ray::rlDisableFramebuffer();
		}
		else TRACELOG(LOG_WARNING, "FBO: Framebuffer object can not be created");
		return _id > 0;
	}

}