# CMakeLists.txt : CMake project for AlgeEngine, include source and define
# project specific logic here.

# Add source to this project's executable.
add_executable(AlgeEngine 
    "AlgeEngine.cpp" 
    "AlgeEngine.h"
 "TriangleMesh.cpp" "shader.h" "config.cpp" "material.h" "material.cpp" "linear_algebra.h" "linear_algebra.cpp")

# Set the path for the vcpkg dependencies
set(CMAKE_PREFIX_PATH "C:/Users/Algebra/source/repos/AlgeEngine/vcpkg_installed/x64-windows/share" ${CMAKE_PREFIX_PATH})

# Set the C++ standard to C++20 if the version is greater than 3.12
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET AlgeEngine PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
find_package(glad CONFIG REQUIRED)
target_link_libraries(AlgeEngine PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(AlgeEngine PRIVATE glfw)

find_package(OpenGL REQUIRED)
target_link_libraries(AlgeEngine PRIVATE OpenGL::GL)

# Collect all texture files in the AlgeEngine/textures directory
file(GLOB TEXTURE_FILES "${CMAKE_SOURCE_DIR}/AlgeEngine/textures/*")
# Add custom command to copy the textures after the build
add_custom_command(TARGET AlgeEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:AlgeEngine>/textures"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/AlgeEngine/textures
        $<TARGET_FILE_DIR:AlgeEngine>/textures
    DEPENDS ${TEXTURE_FILES}
)

# Collect all shader files in the AlgeEngine/shaders directory
file(GLOB SHADER_FILES "${CMAKE_SOURCE_DIR}/AlgeEngine/shaders/*")

# Add custom command to copy the shaders after the build
add_custom_command(TARGET AlgeEngine POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E make_directory "$<TARGET_FILE_DIR:AlgeEngine>/shaders"
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/AlgeEngine/shaders
        $<TARGET_FILE_DIR:AlgeEngine>/shaders
    DEPENDS ${SHADER_FILES}
)
