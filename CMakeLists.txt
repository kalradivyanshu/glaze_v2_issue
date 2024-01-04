cmake_minimum_required(VERSION 3.15)
project(GlazeTest)
set (CMAKE_CXX_STANDARD 20)

find_package(glaze REQUIRED)
find_package(refl-cpp)

add_executable(${PROJECT_NAME} src/main.cc src/writer.cc)

target_link_libraries(${PROJECT_NAME} PRIVATE refl-cpp::refl-cpp glaze::glaze)
