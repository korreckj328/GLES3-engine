cmake_minimum_required(VERSION 3.16)
project(simple-game-engine)

set(CMAKE_CXX_STANDARD 14)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake-modules)

find_package(OpenGLES REQUIRED V3)

add_subdirectory(SDL)
include_directories("${CMAKE_SOURCE_DIR}/SDL/include")

file(GLOB PROJECT_SOURCES src/*.h
                          src/*.cpp
                          src/Animation/*.h
                          src/Animation/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})


target_link_libraries(${PROJECT_NAME} OpenGLES::OpenGLESv3)

target_link_libraries(${PROJECT_NAME} SDL2main SDL2-static)

set (source "${CMAKE_SOURCE_DIR}/Assets")

set (destination "${CMAKE_CURRENT_BINARY_DIR}/Assets")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "copy resources folder from ${source} => ${destination}"
)