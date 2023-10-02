#include "Renderer.hpp"

namespace box
{
	Renderer::Renderer()
	{
	}

	Renderer::~Renderer()
	{
	}

	Texture::Texture()
	{
	}

	Texture::~Texture()
	{
		clear();
	}

	bool Texture::load(const char* path)
	{
		clear();
		_data = ray::LoadTexture(path);
		_size.x = _data.width;
		_size.y = _data.height;
		return !empty();
	}

	bool Texture::load(const IImage& image)
	{
		clear();
		_data = ray::LoadTextureFromImage((const ray::Image&)image.data());
		_size.x = _data.width;
		_size.y = _data.height;
		return !empty();
	}

	void Texture::clear()
	{
		if (!empty())
		{
			ray::UnloadTexture(_data);
			_data = {};
			_size = {};
		}
	}

	bool Texture::empty() const
	{
		return _data.id == 0;
	}

	void Texture::setFilter(int32_t filter)
	{
		ray::SetTextureFilter(_data, filter);
	}

	void Texture::setTextureWrap(int32_t wrap)
	{
		ray::SetTextureWrap(_data, wrap);
	}

	void Texture::genMipmaps()
	{
		ray::GenTextureMipmaps(&_data);
	}

	const NativeData& Texture::data() const
	{
		return (const NativeData&)_data;
	}

	Image::Image()
	{
	}

	Image::~Image()
	{
	}

	bool Image::load(const char* path)
	{
		clear();
		_data = ray::LoadImage(path);
		_size.x = _data.width;
		_size.y = _data.height;
		return !empty();
	}

	void Image::clear()
	{
		if (!empty())
		{
			ray::UnloadImage(_data);
			_data = {};
			_size = {};
		}
	}

	bool Image::empty() const
	{
		return _data.data == nullptr;
	}

	const NativeData& Image::data() const
	{
		return (const NativeData&)_data;
	}

	RenderTexture::RenderTexture()
	{
	}

	RenderTexture::~RenderTexture()
	{
	}

	bool RenderTexture::create(int32_t w, int32_t h)
	{
		clear();
		_data = ray::LoadRenderTexture(w, h);
		_size.x = w;
		_size.y = h;
		return !empty();
	}

	void RenderTexture::clear()
	{
		if (!empty())
		{
			ray::UnloadRenderTexture(_data);
			_data = {};
		}
	}

	bool RenderTexture::empty() const
	{
		return _data.id == 0;
	}

	const NativeData& RenderTexture::data() const
	{
		return (const NativeData&)_data;
	}
}