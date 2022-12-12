cmake_minimum_required(VERSION 3.15)

project(Renderer)

add_library(${PROJECT_NAME} Camera.h
                            LightManager.cpp
                            PerspectiveCamera.cpp       # Derived from Camera.h
                            OrthographicCamera.cpp      # Derived from Camera.h
                            IndexBuffer.cpp
                            VertexArray.cpp
                            VertexBuffer.cpp
                            BufferLayout.h              # Stores data for VertexArray.cpp
                            Shader.cpp
                            TextureManager.cpp
                            Model.cpp )

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

add_library(Engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glad glm stb Engine::GLFWApplication Engine::ErrorHandler)
