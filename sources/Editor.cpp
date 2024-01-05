#include "Editor.hpp"

#include "Plugin.hpp"

#if defined(_MSC_VER) // Microsoft compiler
#include <windows.h>
#elif defined(__GNUC__) // GNU compiler
#include <dlfcn.h>
#else
#error define your copiler
#endif
#include <chrono>
#include <random>

namespace box
{
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

    void open_default_application(std::string_view file)
    {
#ifdef _WIN32
        ::ShellExecuteW(NULL, L"open", utf8_decode(file).c_str(), NULL, NULL, SW_SHOWNORMAL);
#endif
    }

    uint64_t generate_uuid()
    {
        static std::random_device seed;
        static std::mt19937       PRNG(seed());

        std::uniform_int_distribution<uint32_t> uint_dist_long(0, UINT32_MAX);
        uint64_t                                uuid = uint_dist_long(PRNG);

        std::chrono::nanoseconds ns = std::chrono::duration_cast<std::chrono::nanoseconds>(
            std::chrono::steady_clock::now().time_since_epoch());

        uuid |= (ns.count() << 32);

        return uuid;
    }

    std::string_view show_message(std::string_view title, std::string_view text, show_message_type buttons)
    {
        auto flags = 0;
        switch (buttons)
        {
            case show_message_type::ok_cancel:
                flags = MB_OKCANCEL;
                break;
            case show_message_type::yes_no_cancel:
                flags = MB_YESNOCANCEL;
                break;
            case show_message_type::yes_no:
                flags = MB_YESNO;
                break;
            case show_message_type::abort_retry_ignore:
                flags = MB_ABORTRETRYIGNORE;
                break;
            case show_message_type::retry_cancel:
                flags = MB_RETRYCANCEL;
                break;
            case show_message_type::abort_ignore:
                flags = MB_ABORTRETRYIGNORE;
                break;
            default:
                flags = MB_OK;
        }

        switch (MessageBoxW(0, utf8_decode(text).c_str(), utf8_decode(title).c_str(), flags))
        {
            case IDOK:
                return std::string_view("ok");
            case IDCANCEL:
                return std::string_view("cancel");
            case IDYES:
                return std::string_view("yes");
            case IDNO:
                return std::string_view("no");
            case IDABORT:
                return std::string_view("abort");
            case IDRETRY:
                return std::string_view("retry");
            case IDIGNORE:
                return std::string_view("ignore");
        }
        return std::string_view();
    }
}