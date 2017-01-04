cmake_minimum_required(VERSION 2.8)

project(shadertool)

option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(deps/glfw)

if(MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
    if(NOT WIN32)
        set(GLAD_LIBRARIES dl)
    endif()
endif()

include_directories(include/
                    deps/glad/include/
                    deps/glfw/include/
                    deps/glm/
                    deps/stb/
                    deps/argparser/c/)

file(GLOB DEPS deps/glad/src/glad.c deps/argparser/c/argparser.c)
file(GLOB SOURCES src/*.cpp
                  src/*.c)
file(GLOB HEADERS include/*.hpp
                  include/*.h)
file(GLOB SHADERS shaders/*.comp
                  shaders/*.frag
                  shaders/*.geom
                  shaders/*.vert)
file(GLOB CONFIGS CMakeLists.txt
                  README.md
                  .gitignore
                  .gitmodules)

source_group("Headers" FILES ${HEADERS})
source_group("Shaders" FILES ${SHADERS})
source_group("Sources" FILES ${SOURCES})

add_definitions(-DGLFW_INCLUDE_NONE)
add_definitions(-DPROJECT_ROOT=\"${PROJECT_SOURCE_DIR}\")

add_executable(shadertool src/main.cpp ${SOURCES} ${HEADERS} ${SHADERS} ${CONFIGS} ${DEPS})
target_link_libraries(shadertool glfw ${GLFW_LIBRARIES} ${GLAD_LIBRARIES})
