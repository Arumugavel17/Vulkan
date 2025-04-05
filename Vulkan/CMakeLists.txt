
include_directories($ENV{VULKAN_SDK}/Include)
link_directories($ENV{VULKAN_SDK}/Lib)

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
target_link_libraries(Vulkan
        glfw
		vulkan-1)

add_subdirectory ("vendor/glm")
add_subdirectory ("vendor/glfw")
include_directories("vendor/glfw/include")
include_directories("vendor/glm")

if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET Vulkan PROPERTY CXX_STANDARD 20)
endif()

# TODO: Add tests and install targets if needed.
