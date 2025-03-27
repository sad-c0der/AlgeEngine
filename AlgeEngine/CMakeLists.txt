# CMakeList.txt : CMake project for AlgeEngine, include source and define
# project specific logic here.
#

# Add source to this project's executable.
add_executable (AlgeEngine "AlgeEngine.cpp" "AlgeEngine.h")

set(CMAKE_PREFIX_PATH "C:/Users/Algebra/source/repos/AlgeEngine/vcpkg_installed/x64-windows/share" ${CMAKE_PREFIX_PATH})

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

