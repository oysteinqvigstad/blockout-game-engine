cmake_minimum_required(VERSION 3.15)

project(ErrorHandler)

add_library(${PROJECT_NAME} ErrorHandler.cpp)

add_library(Engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(${PROJECT_NAME} PRIVATE glad glfw)