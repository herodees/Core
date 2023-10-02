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
		return !empty();
	}

	bool Texture::load(const IImage& image)
	{
		clear();
		_data = ray::LoadTextureFromImage((const ray::Image&)image.data());
		return !empty();
	}

	void Texture::clear()
	{
		if (!empty())
		{
			ray::UnloadTexture(_data);
			_data = {};
		}
	}

	bool Texture::empty() const
	{
		return _data.id == 0;
	}

	int32_t Texture::width() const
	{
		return _data.width;
	}

	int32_t Texture::height() const
	{
		return _data.height;
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
		return !empty();
	}

	void Image::clear()
	{
		if (!empty())
		{
			ray::UnloadImage(_data);
			_data = {};
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

	int32_t Image::width() const
	{
		return _data.width;
	}

	int32_t Image::height() const
	{
		return _data.height;
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

	int32_t RenderTexture::width() const
	{
		return _data.texture.width;
	}

	int32_t RenderTexture::height() const
	{
		return _data.texture.height;
	}

	const NativeData& RenderTexture::data() const
	{
		return (const NativeData&)_data;
	}
}