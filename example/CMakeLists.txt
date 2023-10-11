cmake_minimum_required(VERSION 3.8.2) # 3.8.0 is minimum version that allow Linux remote building and debugging
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Remove trailing '\' and '/' at the end of the path, so can reliably compare
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_SOURCE_DIR ${CMAKE_SOURCE_DIR})
STRING(REGEX REPLACE "[/\\]+$" "" CMAKE_CURRENT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR})
   
set(MODULE_FILES
	d-hello.cpp
	d-hello.hpp
)


add_library(DLib SHARED ${MODULE_FILES})

# C++ hardcode options
target_compile_features(DLib PUBLIC cxx_std_20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(DLib PUBLIC -Wno-c++98-compat -Wno-c++98-compat-pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(DLib PUBLIC -Wall)
endif()

#include(GenerateExportHeader)
#generate_export_header(DLib)


# Make sure the compiler can find include files for our Hello library
# when other libraries or executables link to Hello
target_include_directories(DLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_include_directories(DLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../include)

# List files that install and/or package should provide.
# Each library should know what it want to distribute,
# which files are internal or intermediate and which are public library export.
install(TARGETS DLib DESTINATION DLib-bin)
install(FILES d-hello.hpp DESTINATION DLib-include)

