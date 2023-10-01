cmake_minimum_required(VERSION 3.20)
project(main)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Dependencies
## -- OpenGL
if (UNIX)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif(UNIX)

# Adding Raylib
include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

FetchContent_Populate(imgui
  URL https://github.com/ocornut/imgui/archive/docking.zip
)

# Adding our source files
file(GLOB_RECURSE PROJECT_SOURCES CONFIGURE_DEPENDS "${CMAKE_CURRENT_LIST_DIR}/sources/*.cpp") # Define PROJECT_SOURCES as a list of all source files
set(PROJECT_SOURCE "${CMAKE_CURRENT_LIST_DIR}/sources/") # Define PROJECT_INCLUDE to be the path to the include directory of the project
set(PROJECT_INCLUDE "${CMAKE_CURRENT_LIST_DIR}/include/")

# Declaring our executable
add_executable(${PROJECT_NAME})
target_sources(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCES})
target_sources(${PROJECT_NAME} PRIVATE 
  ${imgui_SOURCE_DIR}/imgui.cpp
  ${imgui_SOURCE_DIR}/imgui_draw.cpp
  ${imgui_SOURCE_DIR}/imgui_demo.cpp
  ${imgui_SOURCE_DIR}/imgui_tables.cpp
  ${imgui_SOURCE_DIR}/imgui_widgets.cpp
  ${imgui_SOURCE_DIR}/misc/cpp/imgui_stdlib.cpp
)

target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_INCLUDE})
target_include_directories(${PROJECT_NAME} PRIVATE ${PROJECT_SOURCE})
target_include_directories(${PROJECT_NAME} PRIVATE ${imgui_SOURCE_DIR})
target_include_directories(${PROJECT_NAME} PRIVATE "${imgui_SOURCE_DIR}/backendse")
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

message("---->" ${imgui_SOURCE_DIR})

# Setting ASSETS_PATH
target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/") # Set the asset path macro to the absolute path on the dev machine
#target_compile_definitions(${PROJECT_NAME} PUBLIC ASSETS_PATH="./assets") # Set the asset path macro in release mode to a relative path that assumes the assets folder is in the same directory as the game executable

# C++ hardcode options
target_compile_features(${CMAKE_PROJECT_NAME} PUBLIC cxx_std_20)
if (CMAKE_CXX_COMPILER_ID STREQUAL "Clang")
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wno-c++98-compat -Wno-c++98-compat-pedantic)
elseif (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
    target_compile_options(${CMAKE_PROJECT_NAME} PUBLIC -Wall)
endif()

# Web Configurations
if (${PLATFORM} STREQUAL "Web")
    # Tell Emscripten to build an example.html file.
    set_target_properties(${PROJECT_NAME} PROPERTIES SUFFIX ".html")
endif()

# Checks if OSX and links appropriate frameworks (Only required on MacOS)
if (APPLE)
    target_link_libraries(${PROJECT_NAME} "-framework IOKit")
    target_link_libraries(${PROJECT_NAME} "-framework Cocoa")
    target_link_libraries(${PROJECT_NAME} "-framework OpenGL")
endif()
