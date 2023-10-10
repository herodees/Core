#pragma once

namespace box
{
	struct Vertex;
	enum class BlendMode;
	enum class UniformType;

	class IAsset
	{
	public:
		virtual ~IAsset() = default;
		virtual bool save(const char* path) = 0;
		virtual bool load(const char* path) = 0;

	public:
		void add_ref() { ++_ref; }
		bool rem_ref() { if (!--_ref); { delete this; return true; } return false; }

	private:
		std::atomic_int _ref{1};
	};



	class IMaterial : public IAsset
	{
	public:
		virtual ~IMaterial() = default;
		virtual void bind(bool activate) = 0;
		virtual void draw(const Vertex* vtx, size_t size) = 0;
		virtual void set_shader(const char* vs, const char* fs) = 0;
		virtual void set_blend_mode(BlendMode blend) = 0;
		virtual void set_texture(uint32_t loc, uint32_t texture) = 0;
		virtual void set_uniform(uint32_t loc, const void* data, uint32_t count, UniformType type) = 0;
		virtual uint32_t get_location(const char* name) const = 0;
	};



	class ITexture : public IAsset
	{
	public:
		virtual ~ITexture() = default;
		const Vec2i& size() const { return _size; }
		uint32_t handle() const { return _id; }
		int32_t get_format() const { return _format; }
		virtual void set_filter(uint32_t filter) = 0;
		virtual void set_wrap(uint32_t wrap) = 0;
		virtual void generate_mipmap() = 0;

	protected:
		uint32_t _id{};
		Vec2i _size{};
		int32_t _mipmaps{};
		int32_t _format{};
	};



	class ISound : public IAsset
	{
	public:
		virtual ~ISound() = default;
	};



	class IPrototype : public IAsset
	{
	public:
		virtual ~IPrototype() = default;
	};



	template <typename AST>
	class AssetRef
	{
	public:
		AssetRef() = default;
		AssetRef(AST* a) : _ptr(a) {}
		AssetRef(const AssetRef<AST>& ref) : _ptr(ref._ptr) { if (_ptr) _ptr->add_ref(); }
		AssetRef(AssetRef<AST>&& ref) : _ptr(ref._ptr) { ref._ptr = nullptr; }
		~AssetRef() { reset(); }
		AssetRef<AST>& operator=(const AssetRef<AST>& c) { if (c._ptr != _ptr) reset(c._ptr); return *this; }
		AssetRef<AST>& operator=(AssetRef<AST>&& c) { std::swap(_ptr, c._ptr); return *this; }
		void reset(AST* ptr = nullptr) { if (_ptr) _ptr->rem_ref(); _ptr=ptr; if (_ptr) _ptr->add_ref(); }

		AST& operator*() { return *_ptr; }
		const AST& operator*() const { return *_ptr; }
		AST* operator->() { return _ptr; }
		const AST* operator->() const { return _ptr; }

		operator AST*() { return _ptr; }
		operator const AST* () const { return _ptr; }

		AST* _ptr{};
	};



	class IAssetProvider
	{
	public:
		virtual ~IAssetProvider() = default;

		virtual AssetRef<ITexture> load_texture(const char* path) = 0;
		virtual AssetRef<IMaterial> load_material(const char* path) = 0;
		virtual AssetRef<ISound> load_sound(const char* path) = 0;
		virtual AssetRef<IPrototype> load_prototype(const char* path) = 0;
	};
}