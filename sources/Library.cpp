
#include "Library.hpp"

#if defined(_MSC_VER) // Microsoft compiler
#include <windows.h>
#elif defined(__GNUC__) // GNU compiler
#include <dlfcn.h>
#else
#error define your copiler
#endif

namespace box
{
	Library::Library()
	{
	}

	Library::Library(Library&& c)
	{
		std::swap(_lib, c._lib);
	}

	Library::~Library()
	{
		clear();
	}

	Library& Library::operator=(Library&& c)
	{
		std::swap(_lib, c._lib);
		return *this;
	}

	void* Library::get_function(const char* function)
	{
		if (!_lib)
			return nullptr;
#if defined(_MSC_VER) // Microsoft compiler
		return (void*)GetProcAddress((HINSTANCE)_lib, function);
#elif defined(__GNUC__) // GNU compiler
		return dlsym(_lib, function);
#endif
	}

	void* Library::load(const char* name, int imode)
	{
		clear();
		std::string sDllName = name;
#if defined(_MSC_VER) // Microsoft compiler
		sDllName += ".dll";
		_lib = (void*)LoadLibrary(name);
#elif defined(__GNUC__) // GNU compiler
		sDllName += ".so";
		_lib =  dlopen(sDllName.c_str(), iMode);
#endif
		return _lib;
	}

	void Library::clear()
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