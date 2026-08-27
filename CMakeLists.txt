cmake_minimum_required(VERSION 4.2)
project(MazeExplorer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin/${CMAKE_BUILD_TYPE})

file(GLOB_RECURSE SOURCES
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(MazeExplorer ${SOURCES})
target_compile_definitions(MazeExplorer PRIVATE
        PLATFORM_GLFW
        RENDERER_OPENGL
)

# Dependencies
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
target_include_directories(MazeExplorer PRIVATE
        deps/glad/include
        deps/glfw/include

        src/
)
target_compile_definitions(MazeExplorer PRIVATE
        GLFW_INCLUDE_NONE
)
target_link_libraries(MazeExplorer glfw glad)
