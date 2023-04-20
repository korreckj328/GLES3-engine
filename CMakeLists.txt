cmake_minimum_required(VERSION 3.16)
project(simple-game-engine)

set(CMAKE_CXX_STANDARD 14)

if (NOT APPLE) 
        find_package(OpenGL)
endif()

find_package(GLEW)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(glfw)
include_directories("${CMAKE_SOURCE_DIR}/glfw/include")

file(GLOB PROJECT_SOURCES src/*.h
                          src/*.cpp
                          src/Animation/*.h
                          src/Animation/*.cpp)

add_executable(${PROJECT_NAME} ${PROJECT_SOURCES})

if (NOT APPLE) 
        target_link_libraries(${PROJECT_NAME} OpenGL)
endif()

target_link_libraries(${PROJECT_NAME} GLEW::GLEW)
target_link_libraries(${PROJECT_NAME} glfw)

set (source "${CMAKE_SOURCE_DIR}/Assets")

set (destination "${CMAKE_CURRENT_BINARY_DIR}/Assets")

add_custom_command(
        TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory ${source} ${destination}
        DEPENDS ${destination}
        COMMENT "copy resources folder from ${source} => ${destination}"
)