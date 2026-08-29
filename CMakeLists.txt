cmake_minimum_required(VERSION 3.20)
project(MazeExplorer)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

# Output directories
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin/$<CONFIG>")

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/src/*.cpp"
)

add_executable(MazeExplorer ${SOURCES})
target_compile_definitions(MazeExplorer PRIVATE
        PLATFORM_GLFW
        RENDERER_OPENGL
)

# Copy assets to output directory
file(GLOB_RECURSE ASSETS CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/assets/*"
)

add_custom_target(copy_assets ALL
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${CMAKE_SOURCE_DIR}/assets"
        "$<TARGET_FILE_DIR:MazeExplorer>/assets"
        DEPENDS ${ASSETS}
        COMMENT "Copying assets"
)

add_dependencies(MazeExplorer copy_assets)

# Dependencies
set(GLFW_BUILD_DOCS OFF)
set(GLFW_BUILD_EXAMPLES OFF)
set(GLFW_BUILD_TESTS OFF)
set(GLFW_INSTALL OFF)
add_subdirectory(deps/glfw)
add_subdirectory(deps/glad)
add_subdirectory(deps/glm)
target_include_directories(MazeExplorer PRIVATE
        deps/glad/include
        deps/glfw/include
        deps/glm/include

        src/
)
target_compile_definitions(MazeExplorer PRIVATE
        GLFW_INCLUDE_NONE
)
target_link_libraries(MazeExplorer
        glfw
        glad
        glm
)
