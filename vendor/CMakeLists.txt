include(FetchContent)
set(FETCHCONTENT_QUIET FALSE)

add_compile_definitions(ENTT_NOEXCEPTION)
FetchContent_Declare(
    entt
    GIT_REPOSITORY https://github.com/skypjack/entt.git
    GIT_TAG v3.11.1)
FetchContent_MakeAvailable(entt)

## -- OpenGL
if (UNIX)
    set(OpenGL_GL_PREFERENCE LEGACY)
endif(UNIX)

# Adding Raylib
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) # don't build the supplied examples
set(BUILD_GAMES    OFF CACHE BOOL "" FORCE) # don't build the supplied example games

FetchContent_Declare(
    raylib
    GIT_REPOSITORY "https://github.com/raysan5/raylib.git"
    GIT_TAG "master"
    GIT_PROGRESS TRUE
)

FetchContent_MakeAvailable(raylib)

add_subdirectory(chipmunk)
add_subdirectory(imgui)
add_subdirectory(stb)
