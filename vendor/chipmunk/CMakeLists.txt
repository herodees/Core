# use float instead of double for chipmunk; we don't need the extra precision,
# and it matches what glm and ImGui use by default.
add_compile_definitions(CP_USE_DOUBLES=0 CP_USE_CGTYPES=0)

if(NOT MSVC)
  set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -std=gnu99") # always use gnu99
  if(FORCE_CLANG_BLOCKS)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fblocks")
  endif()
  set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS_RELEASE} -ffast-math") # extend release-profile with fast-math
  set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -Wall") # extend debug-profile with -Wall
endif()


FetchContent_Populate(
    chipmunk
    GIT_REPOSITORY https://github.com/slembcke/Chipmunk2D
    GIT_TAG Chipmunk-7.0.3
    GIT_PROGRESS TRUE)


file(GLOB chipmunk_source_files "${chipmunk_SOURCE_DIR}/src/*.c")
file(GLOB chipmunk_public_header "${chipmunk_SOURCE_DIR}/include/chipmunk/*.h")

add_library(chipmunk STATIC
    ${chipmunk_source_files}
)
target_include_directories(chipmunk PRIVATE ${chipmunk_SOURCE_DIR}/include)

if(MSVC)
    set_source_files_properties(${chipmunk_source_files} PROPERTIES LANGUAGE CXX)
    set_target_properties(chipmunk PROPERTIES LINKER_LANGUAGE CXX)
    set_target_properties(chipmunk PROPERTIES CXX_STANDARD 11)
endif(MSVC)

target_include_directories(chipmunk INTERFACE ${chipmunk_SOURCE_DIR}/include)

