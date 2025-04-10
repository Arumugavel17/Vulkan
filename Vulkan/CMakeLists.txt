
include_directories($ENV{VULKAN_SDK}/include)
link_directories($ENV{VULKAN_SDK}/lib)

# Add source to this project's executable.
file(GLOB IMGUI_SOURCES "vendor/imgui/*.cpp" "vendor/imgui/*.h")

file(GLOB_RECURSE SRC_SOURCES 
    src/**/*.cpp)

file(GLOB_RECURSE SRC_HEADERS 
    src/**/*.hpp)

add_executable (Vulkan 
	${IMGUI_SOURCES}
	${SRC_SOURCES}
	${SRC_HEADERS})

message(${CMAKE_SOURCE_DIR})

if(WIN32)
    set(VULKAN_LIB vulkan-1)
    target_compile_definitions(Vulkan PRIVATE PLT_WINDOWS)
    message(STATUS "Running on Windows")
elseif(UNIX AND NOT APPLE)
    set(VULKAN_LIB vulkan)
    target_compile_definitions(Vulkan PRIVATE PLT_UNIX)
    message(STATUS "Running on Linux")
elseif(APPLE)
    target_compile_definitions(Vulkan PRIVATE PLT_MAC)
    message(STATUS "Running on macOS")
else()
    message(STATUS "Unknown OS")
endif()

target_link_libraries(Vulkan
        glfw
		${VULKAN_LIB})

add_subdirectory ("vendor/glm")
add_subdirectory ("vendor/glfw")
include_directories("vendor/glfw/include")
include_directories("vendor/glm")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vulkan PROPERTY CXX_STANDARD 20)
endif()
