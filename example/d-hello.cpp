#include "d-hello.hpp"
#include <iostream>

box::plugin* plugin_main(box::game* game)
{
	std::cout << "DynamicLibHello helloFun() just called" << std::endl;

	return nullptr;
}