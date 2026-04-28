cmake_minimum_required(VERSION 3.30)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_MODULE_STD 1)
project(LEARNVULKAN)


#DEPS---------------------------------------------------------------------------
list(APPEND CMAKE_MODULE_PATH "${CMAKE_HOME_DIRECTORY}/cmake/modules")

find_package (Vulkan REQUIRED)
find_package (glfw3 REQUIRED)
find_package (Stb REQUIRED)
find_package (tinyobjloader REQUIRED)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)
set(HEADER_DIR ${CMAKE_SOURCE_DIR}/include)

#cmake util function definitions
include(${CMAKE_SOURCE_DIR}/cmake/utils/compileShader.cmake)

#shader source files
set(SHADER_SLANG_SOURCES ${CMAKE_SOURCE_DIR}/shaders/vertAndFrag.slang)

add_slang_shader_target(mainShaders SOURCES ${SHADER_SLANG_SOURCES})

add_executable(mainTarget)
add_dependencies(mainTarget mainShaders)

#include directories
target_include_directories(mainTarget PRIVATE ${HEADER_DIR})

#defines----------------------------------------------------------------------------------------
target_compile_definitions(mainTarget PRIVATE VULKAN_HPP_NO_STRUCT_CONSTRUCTORS) #CPP 20 feature
target_compile_definitions(mainTarget PRIVATE VULKAN_HPP_HANDLE_ERROR_OUT_OF_DATE_AS_SUCCESS)
target_compile_definitions(mainTarget PRIVATE VK_USE_PLATFORM_WIN32_KHR GLFW_INCLUDE_VULKAN)
target_compile_definitions(mainTarget PRIVATE GLM_FORCE_DEPTH_ZERO_TO_ONE)
target_compile_definitions(mainTarget PRIVATE NOMINMAX)

#Headers for root
target_include_directories(mainTarget PRIVATE ${Stb_INCLUDE_DIR})

#precompiles
target_precompile_headers(mainTarget PRIVATE <iostream> <string> <vector> <array> <cstdlib> <algorithm> <memory> <stdexcept> <vulkan/vulkan_raii.hpp> <GLFW/glfw3.h>)

#EXTRA SOURCES----------------------------------
target_sources( mainTarget PRIVATE 
    ${SRC_DIR}/main.cpp #entry point
    ${SRC_DIR}/celerEngine.cpp
    ${SRC_DIR}/vulkanInstance.cpp
    ${SRC_DIR}/extensions.cpp
    ${SRC_DIR}/window.cpp
    ${SRC_DIR}/swapchain.cpp
    ${SRC_DIR}/pipeline.cpp
    ${SRC_DIR}/pipelineBuilder.cpp
)

target_sources( mainTarget PRIVATE 
    ${HEADER_DIR}/celerEngine.hpp
    ${HEADER_DIR}/vulkanInstance.hpp
    ${HEADER_DIR}/contexts.hpp
    ${HEADER_DIR}/extensions.hpp
    ${HEADER_DIR}/window.hpp
    ${HEADER_DIR}/swapchain.hpp
    ${HEADER_DIR}/pipeline.hpp
    ${HEADER_DIR}/pipelineBuilder.hpp
)

#dependencies------------------------------------------------------------------------------------
target_link_libraries(mainTarget PRIVATE Vulkan::Vulkan)
target_link_libraries(mainTarget PRIVATE tinyobjloader::tinyobjloader)
target_link_libraries(mainTarget PRIVATE glfw)
#target_link_libraries(mainTarget PRIVATE utils)
#target_link_libraries(mainTarget PRIVATE io)