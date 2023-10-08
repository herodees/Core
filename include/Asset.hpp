#pragma once

namespace box
{
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

		AST* _ptr{};
	};



	class IAssetProvider
	{
	public:
		virtual ~IAssetProvider() = default;

		virtual AssetRef<IAsset> load_texture(const char* path) = 0;
		virtual AssetRef<IAsset> load_material(const char* path) = 0;
	};
}