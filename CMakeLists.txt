cmake_minimum_required(VERSION 3.5.0)
project(Template VERSION 0.1.0 LANGUAGES C CXX)

add_executable(Template main.cpp)

include(CTest)
enable_testing()

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)

if(WIN32) # Need to compile statically to prevent Windows .dll not found error for glfw
set(RESOURCE_PATH "$<IF:$<CONFIG:Debug>,Debug,Release>")
set(VCPKG_HOST_TRIPLET x64-windows-static)
set(VCPKG_TARGET_TRIPLET x64-windows-static)
endif()

# adding the custom target forces cmake to copy the Resources folder into the build directory where the executable can access it.
add_custom_target(copy_assets
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/Resources ${CMAKE_CURRENT_BINARY_DIR}/${RESOURCE_PATH}/Resources
    )
add_dependencies(Template copy_assets)



include(./vcpkg/scripts/buildsystems/vcpkg.cmake) # Show where cmake can find vcpkg

find_package(glad CONFIG REQUIRED)
target_link_libraries(Template PRIVATE glad::glad)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(Template PRIVATE glfw)