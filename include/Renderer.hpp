#pragma once

namespace box
{
	struct NativeData {};

	class IRenderer
	{
	public:
		virtual ~IRenderer() = default;
	};

	struct BaseBitmap
	{
		Vec2u _size;
	};

	class IImage : protected BaseBitmap
	{
	public:
		Vec2u size() const { return _size; }
		virtual ~IImage() = default;
		virtual bool load(const char* path) = 0;
		virtual void clear() = 0;
		virtual bool empty() const = 0;
		virtual const NativeData& data() const = 0;
	};

	class ITexture : protected BaseBitmap
	{
	public:
		Vec2u size() const { return _size; }
		virtual ~ITexture() = default;
		virtual const NativeData& data() const = 0;
		virtual bool load(const char* path) = 0;
		virtual bool load(const IImage& image) = 0;
		virtual void clear() = 0;
		virtual bool empty() const = 0;
		virtual void setFilter(int32_t filter) = 0;
		virtual void setTextureWrap(int32_t wrap) = 0;
		virtual void genMipmaps() = 0;
	};

	class IRenderTexture : protected BaseBitmap
	{
	public:
		Vec2u size() const { return _size; }
		virtual ~IRenderTexture() = default;
		virtual bool create(int32_t w, int32_t h) = 0;
		virtual void clear() = 0;
		virtual bool empty() const = 0;
		virtual const NativeData& data() const = 0;
	};
}