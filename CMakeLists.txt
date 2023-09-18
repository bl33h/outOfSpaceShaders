cmake_minimum_required(VERSION 3.26)
project(CG)

# Set C++ standard
set(CMAKE_CXX_STANDARD 14)

# Find required packages
find_package(SDL2 REQUIRED)
find_package(GLEW REQUIRED)
find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)

# Add include directories
include_directories(${SDL2_INCLUDE_DIRS} ${GLEW_INCLUDE_DIRS} ${OPENGL_INCLUDE_DIR})

# Gather all the cpp files
file(GLOB SOURCE_FILES
        "src/*.cpp")


# Add the executable
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

# Link libraries to the executable
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARIES} ${GLEW_LIBRARIES} ${OPENGL_LIBRARIES})
