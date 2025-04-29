cmake_minimum_required(VERSION 3.6)
project(ProjectTitle)

set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# GTest - Gebruik kant en klare libraries
include_directories(gtest/include)
link_directories(gtest/lib)

# Set source files for RELEASE target
set(RELEASE_SOURCE_FILES
        src/TraficObjects/baan.cpp
        src/Situation/situatie.cpp
        src/TraficObjects/verkeerslicht.cpp
        src/TraficObjects/voertuig.cpp
        src/Main/main_release.cpp
        src/Parser/tinyxml.cpp
        src/Parser/tinystr.cpp
        src/Parser/tinyxmlerror.cpp
        src/Parser/tinyxmlparser.cpp
        src/Simulation/simulatie.cpp
        src/Output/output.cpp
        src/FileReader/bestandslezer.cpp
        src/TraficObjects/bushalte.cpp
        src/TraficObjects/kruispunt.cpp
        src/TraficObjects/voertuiggenerator.cpp
)

# Set source files for DEBUG target
set(DEBUG_SOURCE_FILES
        src/TraficObjects/baan.cpp
        src/Situation/situatie.cpp
        src/TraficObjects/verkeerslicht.cpp
        src/TraficObjects/voertuig.cpp
        src/Main/main_debug.cpp
        src/Parser/tinyxml.cpp
        src/Parser/tinystr.cpp
        src/Parser/tinyxmlerror.cpp
        src/Parser/tinyxmlparser.cpp
        src/Simulation/simulatie.cpp
        src/FileReader/bestandslezer.cpp
        src/Output/output.cpp
        src/TraficObjects/voertuiggenerator.cpp
        src/TraficObjects/bushalte.cpp
        src/TraficObjects/kruispunt.cpp
)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})

# Link de GTest libraries
target_link_libraries(debug_target gtest gtest_main)
