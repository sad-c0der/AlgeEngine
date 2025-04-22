# CMakeLists.txt : CMake project for AlgeEngine

cmake_minimum_required(VERSION 3.12)
project(AlgeEngine)

# Add source files
add_executable(AlgeEngine 
    AlgeEngine.cpp 
    config.cpp config.h
    controller/app.cpp controller/app.h
    components/camera_component.h
    components/physics_component.h
    components/render_component.h
    components/transform_component.h
    systems/camera_system.cpp systems/camera_system.h
    systems/motion_system.cpp systems/motion_system.h
    systems/render_system.cpp systems/render_system.h
    view/shader.cpp view/shader.h
    factories/factory.cpp factories/factory.h
)

# Set vcpkg path
set(CMAKE_PREFIX_PATH "C:/Users/Algebra/source/repos/AlgeEngine/vcpkg_installed/x64-windows/share" ${CMAKE_PREFIX_PATH})

# Use C++20
if (CMAKE_VERSION VERSION_GREATER 3.12)
    set_property(TARGET AlgeEngine PROPERTY CXX_STANDARD 20)
endif()

# Dependencies
find_package(glad CONFIG REQUIRED)
find_package(glm CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(OpenGL REQUIRED)

target_link_libraries(AlgeEngine PRIVATE 
    glad::glad 
    glm::glm 
    glfw 
    OpenGL::GL
)

# Function to copy assets
function(copy_assets asset_type)
    set(SRC_DIR "${CMAKE_SOURCE_DIR}/AlgeEngine/${asset_type}")
    set(DST_DIR "$<TARGET_FILE_DIR:AlgeEngine>/${asset_type}")
    file(GLOB_RECURSE ASSET_FILES "${SRC_DIR}/*")
    
    add_custom_command(TARGET AlgeEngine POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E make_directory "${DST_DIR}"
        COMMAND ${CMAKE_COMMAND} -E copy_directory "${SRC_DIR}" "${DST_DIR}"
        DEPENDS ${ASSET_FILES}
    )
endfunction()

# Copy assets after build
copy_assets(models)
copy_assets(textures)
copy_assets(shaders)