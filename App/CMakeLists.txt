project(BlockOut)

add_executable(${PROJECT_NAME} main.cpp Application.cpp ActiveBlock.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw glm stb TCLAP Engine::GLFWApplication Engine::Renderer Engine::GeometricTools Engine::Input)

# Ensure that STB is active
target_compile_definitions(${PROJECT_NAME} PRIVATE STB_IMAGE_IMPLEMENTATION)

# Copy textures to binary directory

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/transparent-box.png
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/transparent-box.png)

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/walls.jpg
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/walls.jpg)
add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_CURRENT_SOURCE_DIR}/resources/textures/cube.jpg
        ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/cube.jpg)

# Add path as a define in Renderer for TextureManager.cpp
target_compile_definitions(Renderer PRIVATE
        TEXTURES_DIR="${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/resources/textures/")
