cmake_minimum_required(VERSION 3.15)
project(PROG2002)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)     # output archive artifacts
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)     # output libraries
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)     # output runtime files

# Including external libraries
add_subdirectory(external/tclap)
add_subdirectory(external/glfw)
add_subdirectory(external/glad)
add_subdirectory(external/glm)
add_subdirectory(external/stb)

# Including core application
add_subdirectory(Engine)
add_subdirectory(App)