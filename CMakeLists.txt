#
# Specify minimum CMake version and project name
#
cmake_minimum_required(VERSION 3.0)
project(JetSimulator)

#
# CMake setup
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set(CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++14")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

#
# Add files
#
file(GLOB_RECURSE PROJECT_HEADERS include/*.hpp
                                  include/*.h)
file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                  src/*.cxx
                                  src/*.cc
                                  src/*.c
                                  vendor/*.cpp
                                  vendor/*.cxx
                                  vendor/*.cc
                                  vendor/*.c)

#
# Main Target
#
add_executable(${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS})
target_include_directories(${PROJECT_NAME} PRIVATE include/ vendor/glad/include/ vendor/glm/include/)

# TODO: automate adding libraries directories to .vscode/c_cpp_properties.json

# 
# GLFW
#
find_package(glfw3 3.2 REQUIRED)
target_link_libraries(${PROJECT_NAME} glfw)
target_include_directories(${PROJECT_NAME} PRIVATE ${GLFW_INCLUDE_DIRS})
add_definitions (-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

#
# DL LIBS
#
target_link_libraries(${PROJECT_NAME} ${CMAKE_DL_LIBS})

#
# Freetype
#
find_package(Freetype REQUIRED)
target_link_libraries(${PROJECT_NAME} ${FREETYPE_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${FREETYPE_INCLUDE_DIRS})

#
# Assimp
#
find_package(Assimp REQUIRED)
target_link_libraries(${PROJECT_NAME} ${ASSIMP_LIBRARIES})
target_include_directories(${PROJECT_NAME} PRIVATE ${ASSIMP_INCLUDE_DIRS})

#
# stb
#
target_link_libraries(${PROJECT_NAME} stb)
target_include_directories(${PROJECT_NAME} PRIVATE /usr/include/stb/)

#
# Link Assets
#
ADD_CUSTOM_TARGET(link_assets ALL
                  COMMAND ${CMAKE_COMMAND} -E create_symlink ${CMAKE_SOURCE_DIR}/assets ./assets)
