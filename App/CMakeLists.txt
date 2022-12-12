project(App)

add_executable(${PROJECT_NAME} main.cpp LabApplication.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw glm stb TCLAP Engine::GLFWApplication Engine::Renderer Engine::GeometricTools Engine::Input)

# Ensure that STB is active
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)


# Copy textures to binary directory
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/floor_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/floor_texture.png)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube_texture.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube_texture.png)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/objects/cube.obj
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/objects/cube.obj)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/marble.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/marble.png)
# Add path as a define in Renderer for TextureManager.cpp
target_compile_definitions(Renderer PRIVATE
        TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")
target_compile_definitions(Renderer PRIVATE
        OBJECTS_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/objects/")
