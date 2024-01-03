cmake_minimum_required(VERSION 3.15)
project(GlazeTest)
set (CMAKE_CXX_STANDARD 20)

find_package(glaze REQUIRED)

add_executable(${PROJECT_NAME} src/main.cc )

target_link_libraries(${PROJECT_NAME} PRIVATE glaze::glaze)
