﻿set(CMAKE_CXX_STANDARD 17)

if(UNIX AND NOT APPLE)
    add_compile_options(-march=x86-64 -mtune=generic -O2)
endif()

add_subdirectory("vendor/glfw")
add_subdirectory("vendor/glm")

include_directories("vendor/glfw/include")
include_directories("vendor/glm")
include_directories("vendor/stb_image")
include_directories("vendor/tinyobjloader")
include_directories("vendor/json_nlohmann")
include_directories("vendor/spirv_reflect")
include_directories("src/")
include_directories("src/Core/")

include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

file(GLOB VENDOR_SOURCES "vendor/**/*.cpp" "vendor/**/*.h")
file(GLOB_RECURSE SRC_SOURCES "src/**/*.cpp" "src/*.cpp")
file(GLOB_RECURSE SRC_HEADERS "src/**/*.h" "src/*.h")

add_executable(VulkanEngine
    ${VENDOR_SOURCES}
    ${SRC_SOURCES}
    ${SRC_HEADERS}
    "main.cpp")

message("Source :" ${CMAKE_CURRENT_SOURCE_DIR})
target_compile_definitions(VulkanEngine PRIVATE CHIKU_SRC_PATH=${CMAKE_CURRENT_SOURCE_DIR}/)

if(WIN32)
    set(VULKAN_LIB vulkan-1)
    target_compile_definitions(VulkanEngine PRIVATE PLT_WINDOWS)
    message(STATUS "Running on Windows")
elseif(UNIX AND NOT APPLE)
    set(VULKAN_LIB vulkan)
    target_compile_definitions(VulkanEngine PRIVATE PLT_UNIX)
    message(STATUS "Running on Linux")
elseif(APPLE)
    target_compile_definitions(VulkanEngine PRIVATE PLT_MAC)
    message(STATUS "Running on macOS")
else()
    message(STATUS "Unknown OS")
endif()
target_link_libraries(VulkanEngine
    glfw
    ${VULKAN_LIB}
)

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET VulkanEngine PROPERTY CXX_STANDARD 20)
endif()
