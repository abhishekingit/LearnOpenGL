cmake_minimum_required(VERSION 3.27)

project(LearnOpenGL)

set(CMAKE_CXX_STANDARD 20)

add_subdirectory(3rdParty/glfw)
add_subdirectory(3rdParty/glad)
add_subdirectory(3rdParty/glm)
add_subdirectory(3rdParty/assimp)

add_executable(LearnOpenGL src/main.cpp src/Shader.cpp src/Shader.h "src/Mesh.h" "src/Model.h")

target_link_libraries(LearnOpenGL glfw glad glm assimp)

target_include_directories(
	LearnOpenGL PUBLIC
	3rdParty/glad/include
	3rdParty/glfw/include
	3rdParty/stb_imageLoader
	3rdParty/glm
	3rdParty/assimp/include
	
)