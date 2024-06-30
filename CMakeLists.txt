cmake_minimum_required(VERSION 3.23.1 FATAL_ERROR)

set(PROJECT_NAME Runner)
project(${PROJECT_NAME})

add_executable(${PROJECT_NAME} 
	src/main.cpp
	src/stb_image.h
	src/CameraManager.h
	src/OpenGLManager.h
	src/Registrar.h
	src/ResourcesManager.h
	src/ScoreInfo.h
	src/TrackGenerator.h
	src/ShaderProgram.h
	src/Texture.h
	src/CameraManager.cpp
	src/OpenGLManager.cpp
	src/Registrar.cpp
	src/ResourcesManager.cpp
	src/ScoreInfo.cpp
	src/TrackGenerator.cpp
	src/ShaderProgram.cpp
	src/Texture.cpp
	src/TexturesEnum.h
	src/Player.h
	src/Player.cpp
	src/Font.h
	src/Font.cpp
	src/Settings.h
	src/Settings.cpp
)

target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_17)

set(GLFW_BUILD_DOCS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_TESTS OFF CACHE BOOL "" FORCE)
set(GLFW_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE) 
set(GLFW_INSTALL OFF CACHE BOOL "" FORCE) 

add_subdirectory(external/glfw)
target_link_libraries(${PROJECT_NAME} PRIVATE glfw)

add_subdirectory(external/glad)
target_link_libraries(${PROJECT_NAME} PRIVATE glad)

add_subdirectory(external/freetype)
target_link_libraries(${PROJECT_NAME} PRIVATE freetype)

include_directories(external/glm)

set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
					COMMAND ${CMAKE_COMMAND} -E copy_directory
					${CMAKE_SOURCE_DIR}/res $<TARGET_FILE_DIR:${PROJECT_NAME}>/res)