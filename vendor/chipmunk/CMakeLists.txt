# use float instead of double for chipmunk; we don't need the extra precision,
# and it matches what glm and ImGui use by default.
add_compile_definitions(CP_USE_DOUBLES=0 CP_USE_CGTYPES=0)

FetchContent_Populate(
    chipmunk
    GIT_REPOSITORY https://github.com/slembcke/Chipmunk2D
    GIT_TAG Chipmunk-7.0.3
    GIT_PROGRESS TRUE)


file(GLOB_RECURSE CHIPMUNK_PROJECT_SOURCES CONFIGURE_DEPENDS "${chipmunk_SOURCE_DIR}/src/*.c") # Define PROJECT_SOURCES as a list of all source files

add_library(chipmunk STATIC
    ${CHIPMUNK_PROJECT_SOURCES}
)

target_include_directories(chipmunk PUBLIC "${chipmunk_SOURCE_DIR}/include")
target_include_directories(chipmunk PUBLIC "${chipmunk_SOURCE_DIR}/src")

