cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_MODULE_STD 1)
project(LEARNVULKAN)



list(APPEND CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake/modules")

find_package (Vulkan REQUIRED)
find_package (glfw3 REQUIRED)
find_package (Stb REQUIRED)
find_package (tinyobjloader REQUIRED)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

#cmake util function definitions
include(${CMAKE_SOURCE_DIR}/cmake/utils/compileShader.cmake)

#shader source files
set(SHADER_SLANG_SOURCES ${CMAKE_SOURCE_DIR}/shaders/vertAndFrag.slang)

add_slang_shader_target(mainShaders SOURCES ${SHADER_SLANG_SOURCES})

add_executable(mainTarget)
add_dependencies(mainTarget mainShaders)

#defines----------------------------------------------------------------------------------------
target_compile_definitions(mainTarget PRIVATE VULKAN_HPP_NO_STRUCT_CONSTRUCTORS) #CPP 20 feature
target_compile_definitions(mainTarget PRIVATE VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS)
target_compile_definitions(mainTarget PRIVATE VK_USE_PLATFORM_WIN32_KHR GLFW_INCLUDE_VULKAN)
target_compile_definitions(mainTarget PRIVATE GLM_FORCE_DEPTH_ZERO_TO_ONE)

#Headers for root
target_include_directories(mainTarget PRIVATE include)
target_include_directories(mainTarget PRIVATE ${Stb_INCLUDE_DIR})

#EXTRA SOURCES
target_sources( mainTarget PRIVATE 
    ${SRC_DIR}/main.cpp
)

#dependencies------------------------------------------------------------------------------------
target_link_libraries(mainTarget PRIVATE Vulkan::Vulkan)
target_link_libraries(mainTarget PRIVATE tinyobjloader::tinyobjloader)
target_link_libraries(mainTarget PRIVATE glfw)
#target_link_libraries(mainTarget PRIVATE utils)
#target_link_libraries(mainTarget PRIVATE io)