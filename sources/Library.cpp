
#include "Library.hpp"

#if defined(_MSC_VER) // Microsoft compiler
#include <windows.h>
#elif defined(__GNUC__) // GNU compiler
#include <dlfcn.h>
#else
#error define your copiler
#endif


#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_GLOBAL 4


namespace box
{
	game_plugin::game_plugin()
	{
	}

	game_plugin::game_plugin(game_plugin&& c)
	{
		std::swap(_lib, c._lib);
	}

	game_plugin::~game_plugin()
	{
		clear();
	}

	game_plugin& game_plugin::operator=(game_plugin&& c)
	{
		std::swap(_lib, c._lib);
		return *this;
	}

	bool game_plugin::load(const char* name)
	{
		if(!load(name, RTLD_NOW))
			return false;

		_main = (plugin_main)get_function("plugin_main");
		return _main;
	}

	bool game_plugin::is_loaded() const
	{
		return _lib && _main;
	}

	plugin* game_plugin::create(game* g)
	{
		if (!_main)
			return nullptr;
		_plugin = _main(g);
		return _plugin;
	}

	plugin* game_plugin::get()
	{
		return _plugin;
	}

	void* game_plugin::get_function(const char* function)
	{
		if (!_lib)
			return nullptr;
#if defined(_MSC_VER) // Microsoft compiler
		return (void*)GetProcAddress((HINSTANCE)_lib, function);
#elif defined(__GNUC__) // GNU compiler
		return dlsym(_lib, function);
#endif
	}

	void* game_plugin::load(const char* name, int imode)
	{
		clear();
		std::string dllname = name;
#if defined(_MSC_VER) // Microsoft compiler
		dllname += ".dll";
		_lib = (void*)LoadLibrary(name);
#elif defined(__GNUC__) // GNU compiler
		dllname += ".so";
		_lib =  dlopen(dllname.c_str(), imode);
#endif
		return _lib;
	}

	void game_plugin::clear()
	{
		if (!_lib)
			return;
#if defined(_MSC_VER) // Microsoft compiler
		FreeLibrary((HINSTANCE)_lib);
#elif defined(__GNUC__) // GNU compiler
		dlclose(_lib);
#endif
		_lib = nullptr;
	}

}