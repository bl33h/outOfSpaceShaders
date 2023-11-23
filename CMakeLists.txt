cmake_minimum_required(VERSION 3.26)

project(GAME)

set(CMAKE_CXX_STANDARD 17)

find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

set(OpenGL_GL_PREFERENCE "GLVND")

include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

file(GLOB SOURCE_FILES
        "src/*.cpp")

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})