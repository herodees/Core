
#include "Plugin.hpp"

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
        if (dllname.rfind(".dll") == std::string::npos)
			dllname += ".dll";
		_lib = (void*)LoadLibrary(name);
#elif defined(__GNUC__) // GNU compiler
        if (dllname.rfind(".so") == std::string::npos)
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

#ifdef _WIN32
	// Convert a wide Unicode string to an UTF8 string
    std::string utf8_encode(const std::wstring_view& wstr)
    {
        if (wstr.empty())
            return std::string();
        int         size_needed = WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), NULL, 0, NULL, NULL);
        std::string strTo(size_needed, 0);
        WideCharToMultiByte(CP_UTF8, 0, &wstr[0], (int)wstr.size(), &strTo[0], size_needed, NULL, NULL);
        return strTo;
    }

    // Convert an UTF8 string to a wide Unicode String
    std::wstring utf8_decode(const std::string_view& str)
    {
        if (str.empty())
            return std::wstring();
        int          size_needed = MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), NULL, 0);
        std::wstring wstrTo(size_needed, 0);
        MultiByteToWideChar(CP_UTF8, 0, &str[0], (int)str.size(), &wstrTo[0], size_needed);
        return wstrTo;
    }
#endif

	std::vector<std::string> file_dialog(bool save_dialog, std::string_view file, std::string_view filter)
    {
#ifdef _WIN32
        auto wfilter = utf8_decode(filter);
        std::transform(wfilter.begin(),
                       wfilter.end(),
                       wfilter.begin(),
                       [](wchar_t c)
                       {
                           if (c == '|')
                               return L'\0';
                           return c;
                       });
        wfilter.push_back(0);
        auto wfile = utf8_decode(file);
        wfile.resize(1024);

        OPENFILENAMEW ofn         = {0};
        // Initialize remaining fields of OPENFILENAME structure
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = 0;
        ofn.lpstrFile       = wfile.data();
        ofn.nMaxFile        = wfile.size();
        ofn.lpstrFilter     = wfilter.c_str();
        ofn.nFilterIndex    = 1;
        ofn.lpstrFileTitle  = NULL;
        ofn.nMaxFileTitle   = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags           = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;

		if (save_dialog && GetSaveFileNameW(&ofn) == TRUE)
        {
            std::vector<std::string> out;
            out.emplace_back(utf8_encode(ofn.lpstrFile));
            return out;
		}
        if (!save_dialog && GetOpenFileNameW(&ofn) == TRUE)
        {
            std::vector<std::string> out;
            out.emplace_back(utf8_encode(ofn.lpstrFile));
            return out;
        }
#endif
        return std::vector<std::string>();
    }

    std::vector<std::string> open_file_dialog(std::string_view file, bool multiselect, std::string_view filter)
    {
#ifdef _WIN32
        auto wfilter = utf8_decode(filter);
        std::transform(wfilter.begin(),
                       wfilter.end(),
                       wfilter.begin(),
                       [](wchar_t c)
                       {
                           if (c == '|')
                               return L'\0';
                           return c;
                       });
        wfilter.push_back(0);
        auto wfile = utf8_decode(file);
        wfile.resize(1024 * 4);

        OPENFILENAMEW ofn = {0};
        // Initialize remaining fields of OPENFILENAME structure
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = 0;
        ofn.lpstrFile       = wfile.data();
        ofn.nMaxFile        = wfile.size();
        ofn.lpstrFilter     = wfilter.c_str();
        ofn.nFilterIndex    = 1;
        ofn.lpstrFileTitle  = NULL;
        ofn.nMaxFileTitle   = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags           = 0x00080000 | OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST | OFN_NOCHANGEDIR;
        if (multiselect)
            ofn.Flags |= OFN_ALLOWMULTISELECT;

        if (GetOpenFileNameW(&ofn) == TRUE)
        {
            std::vector<std::string> out;
            size_t prev_n = 0;
            for (size_t n = 0; n < wfile.size(); ++n)
            {
                if (!wfile[n])
                {
                    if (n == prev_n)
                        break;
                    out.push_back(utf8_encode({wfile.c_str() + prev_n, wfile.c_str() + n}));
                    prev_n = n + 1;
                }
            }
            if (out.size() > 1)
                out[0].push_back('\\');
            return out;
        }
#endif
        return std::vector<std::string>();
    }

    std::string save_file_dialog(std::string_view file, std::string_view filter)
    {
#ifdef _WIN32
        auto wfilter = utf8_decode(filter);
        std::transform(wfilter.begin(),
                       wfilter.end(),
                       wfilter.begin(),
                       [](wchar_t c)
                       {
                           if (c == '|')
                               return L'\0';
                           return c;
                       });
        wfilter.push_back(0);
        auto wfile = utf8_decode(file);
        wfile.resize(1024);

        OPENFILENAMEW ofn = {0};
        // Initialize remaining fields of OPENFILENAME structure
        ofn.lStructSize     = sizeof(ofn);
        ofn.hwndOwner       = 0;
        ofn.lpstrFile       = wfile.data();
        ofn.nMaxFile        = wfile.size();
        ofn.lpstrFilter     = wfilter.c_str();
        ofn.nFilterIndex    = 1;
        ofn.lpstrFileTitle  = NULL;
        ofn.nMaxFileTitle   = 0;
        ofn.lpstrInitialDir = NULL;
        ofn.Flags           = 0x00080000 | OFN_OVERWRITEPROMPT | OFN_NOCHANGEDIR;

        if (GetSaveFileNameW(&ofn) == TRUE)
        {
            return utf8_encode(ofn.lpstrFile);
        }
#endif
        return std::string();
    }
}