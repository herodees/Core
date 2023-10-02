#pragma once

#include "Include.hpp"

namespace box
{
	class Renderer final : public IRenderer
	{
	public:
		Renderer();
		~Renderer() override;

	private:
	};



	class Image final : public IImage
	{
	public:
		Image();
		~Image() override;
		Image(const Image&) = delete;

		bool load(const char* path) override;
		void clear() override;
		bool empty() const override;

		const NativeData& data() const override;

	protected:
		ray::Image _data{};
	};



	class Texture final : public ITexture
	{
	public:
		Texture();
		~Texture() override;
		Texture(const Texture&) = delete;

		bool load(const char* path) override;
		bool load(const IImage& image) override;
		void clear() override;
		bool empty() const override;

		void setFilter(int32_t filter) override;
		void setTextureWrap(int32_t wrap) override;
		void genMipmaps() override;

		const NativeData& data() const override;

	protected:
		ray::Texture2D _data{};
	};



	class RenderTexture final : public IRenderTexture
	{
	public:
		RenderTexture();
		~RenderTexture() override;
		RenderTexture(const RenderTexture&) = delete;

		bool create(int32_t w, int32_t h) override;
		void clear() override;
		bool empty() const override;

		const NativeData& data() const override;

	protected:
		ray::RenderTexture2D _data{};
	};
}