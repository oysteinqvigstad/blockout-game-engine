cmake_minimum_required(VERSION 3.15)

project(Input)

add_library(${PROJECT_NAME} Keyboard.cpp)

add_library(Engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)