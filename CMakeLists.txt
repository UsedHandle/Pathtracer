cmake_minimum_required(VERSION 3.12)

set(EXE_NAME "better")

project(${EXE_NAME} C CXX)
set(CMAKE_CXX_STANDARD 20)

include(FetchContent)
FetchContent_Declare(
	glm
	GIT_REPOSITORY	https://github.com/g-truc/glm.git
	GIT_TAG 	bf71a834948186f4097caa076cd2663c69a10e1e #refs/tags/1.0.1
)

FetchContent_MakeAvailable(glm)

#find_package(SDL2 REQUIRED)

add_library(stb_image STATIC stb_image_write.c)
add_executable(${EXE_NAME} main.cpp pathtracer.cpp ray.cpp sampler.cpp scene.cpp sphere.cpp triangle.cpp)

if(MSVC)
	target_compile_options(${EXE_NAME} PRIVATE /W4)
else()
	target_compile_options(${EXE_NAME} PRIVATE -Wall -Wextra -Wshadow -Wconversion -Wpedantic -O3)
	target_compile_options(stb_image PRIVATE -O3)
endif()

include_directories(include/)
target_link_libraries(${EXE_NAME} PRIVATE  stb_image glm::glm)
