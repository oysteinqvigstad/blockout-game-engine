cmake_minimum_required(VERSION 3.15)

project(GeometricTools)

add_library(${PROJECT_NAME} INTERFACE)

add_library(Engine::${PROJECT_NAME} ALIAS ${PROJECT_NAME})
target_include_directories(${PROJECT_NAME} INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
