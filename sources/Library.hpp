#pragma once
#include <string>

namespace box
{

/*
#define RTLD_LAZY   1
#define RTLD_NOW    2
#define RTLD_GLOBAL 4
*/
	class Library
	{
	public:
		Library();
		Library(const Library& c) = delete;
		Library(Library&& c);
		~Library();
		Library& operator=(const Library& c) = delete;
		Library& operator=(Library&& c);

		void* load(const char* name, int imode = 2);
		void* get_function(const char* function);
		void clear();

		void* _lib{};
	};


}