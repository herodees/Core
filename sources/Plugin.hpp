#pragma once
#include <string>

namespace box
{
    class plugin;
    class game;

    class game_plugin
    {
    public:
        using plugin_main = plugin* (*)(game*);

        game_plugin();
        game_plugin(const game_plugin& c) = delete;
        game_plugin(game_plugin&& c);
        ~game_plugin();
        game_plugin& operator=(const game_plugin& c) = delete;
        game_plugin& operator=(game_plugin&& c);

        bool    load(const char* name);
        bool    is_loaded() const;
        plugin* create(game* g);
        plugin* get();

    protected:
        void* load(const char* name, int imode);
        void* get_function(const char* function);
        void  clear();

        void*       _lib{};
        plugin_main _main{};
        plugin*     _plugin{};
    };


} // namespace box