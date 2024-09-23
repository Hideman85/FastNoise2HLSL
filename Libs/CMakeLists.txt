option(BUILD_UNIT_TESTS "Build The-Forge's unit tests" OFF)
add_subdirectory(The-Forge)
add_subdirectory(FastNoise2)

add_library(required_libs INTERFACE)
target_link_libraries(required_libs INTERFACE FastNoise2) # Note the forge is set later with its own macro
