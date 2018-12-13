#
# Specify minimum CMake version and project name
#
cmake_minimum_required (VERSION 3.0)
project (JetSimulator)

#
# CMake setup
#
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set (CMAKE_VERBOSE_MAKEFILE 0) # 1 should be used for debugging
set (CMAKE_SUPPRESS_REGENERATION TRUE) # Suppresses ZERO_CHECK
if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
  if(NOT WIN32)
    set(GLAD_LIBRARIES dl)
  endif()
endif()

#
# GLFW options
#
option (GLFW_INSTALL OFF)
option (GLFW_BUILD_DOCS OFF)
option (GLFW_BUILD_EXAMPLES OFF)
option (GLFW_BUILD_TESTS OFF)
add_subdirectory (vendor/glfw)

#
# Set include paths
#
include_directories (src/
                     vendor/glad/include/
                     vendor/glfw/include/
                     vendor/glm/
                     vendor/stb/
                     vendor/assimp/include/
                     vendor/openal/include/)

#
# Add files
#
file (GLOB         VENDORS_SOURCES vendor/glad/src/glad.c)
file (GLOB_RECURSE PROJECT_HEADERS src/*.hpp
                                   src/*.h)
file (GLOB_RECURSE PROJECT_SOURCES src/*.cpp
                                   src/*.cxx
                                   src/*.cc
                                   src/*.c)
file (GLOB         PROJECT_CONFIGS CMakeLists.txt)


#
# Organizing files
#
source_group ("headers" FILES ${PROJECT_HEADERS})
source_group ("sources" FILES ${PROJECT_SOURCES})
source_group ("vendors" FILES ${VENDORS_SOURCES})

#
# Set executable and target link libraries
#
add_definitions (-DGLFW_INCLUDE_NONE
                 -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")
add_executable (${PROJECT_NAME} ${PROJECT_SOURCES} ${PROJECT_HEADERS}
                                ${PROJECT_CONFIGS} ${VENDORS_SOURCES})
target_link_libraries (${PROJECT_NAME}
                       glfw
                       ${GLFW_LIBRARIES}
                       ${GLAD_LIBRARIES})
set_target_properties (${PROJECT_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})



#
# create makefile
#
file (WRITE Makefile "# auto-generated makefile

.PHONY: all cmake build run

all: cmake build run

cmake:
\tcmake -G\"Visual Studio 14 2015 Win64\" -H. -B\"${CMAKE_BINARY_DIR}\" || echo \">>>> cmake initialization failed! <<<<<\"

build:
\tcmake --build \"${CMAKE_BINARY_DIR}\" || echo \">>>> build failed! <<<<<\"

run:
\t\"${CMAKE_BINARY_DIR}/${PROJECT_NAME}/Debug/${PROJECT_NAME}${CMAKE_EXECUTABLE_SUFFIX}\"

clean:
\tcmake --build \"${CMAKE_BINARY_DIR}\" --target clean || echo \">>>> clean failed! <<<<<\"

cleanAll:
\t	del Makefile && rd /s /q build && rd /s /q CMakeFiles || rm -rf build CMakeFiles Makefile
")