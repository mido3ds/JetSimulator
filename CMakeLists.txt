cmake_minimum_required(VERSION 3.9)
project(jetsimulator LANGUAGES CXX VERSION 0.0.1)

# conan
include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
conan_basic_setup()
string(REPLACE ";" " -L" CONAN_LIB_DIRS "${CONAN_LIB_DIRS}")
set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -L${CONAN_LIB_DIRS}")

# GLFW options
add_definitions(-DGLFW_INCLUDE_NONE
            -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

## executable
    add_executable(jetsimulator)
    set_target_properties (jetsimulator PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    # sources
    file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.cxx src/*.cc src/*.c)
    target_sources(jetsimulator PRIVATE ${PROJECT_SOURCES})

    # headers
    target_include_directories(jetsimulator PRIVATE include/)

    file(GLOB_RECURSE PROJECT_HEADERS include/*.hpp include/*.h)
    target_sources(jetsimulator PRIVATE ${PROJECT_HEADERS})

    # conan    
    target_link_libraries(jetsimulator PRIVATE ${CONAN_LIBS})

    # copy files
    file(GLOB_RECURSE ASSETS RELATIVE "${CMAKE_SOURCE_DIR}" assets/*)
    foreach(FILE_ ${ASSETS})
        # message(FATAL_ERROR ${FILE_})
        configure_file(${FILE_} ${FILE_} COPYONLY)
    endforeach()
    
#

## test
    add_executable(test)
    set_target_properties (test PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    # sources
    file(GLOB_RECURSE TEST_SOURCES test/*.cpp test/*.cxx test/*.cc test/*.c)

    file(GLOB_RECURSE PROJECT_SOURCES src/*.cpp src/*.cxx src/*.cc src/*.c)
    file(GLOB_RECURSE MAIN_PATH src/main.*)
    list(REMOVE_ITEM PROJECT_SOURCES "${MAIN_PATH}")

    target_sources(test PRIVATE ${TEST_SOURCES} ${PROJECT_SOURCES})

    # headers
    target_include_directories(test PRIVATE include/)

    file(GLOB_RECURSE PROJECT_HEADERS include/*.hpp include/*.h)
    target_sources(test PRIVATE ${PROJECT_HEADERS})

    # conan    
    target_link_libraries(test PRIVATE ${CONAN_LIBS})
#