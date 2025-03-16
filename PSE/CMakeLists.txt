cmake_minimum_required(VERSION 3.6)
project(ProjectTitle)
set(CMAKE_VERBOSE_MAKEFILE ON)
set(CMAKE_CXX_STANDARD 17)
# Remove the "-O2" flag if you have problems with the debugger.
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Werror -O2")
set(CMAKE_EXE_LINKER_FLAGS -pthread)

# Google Test integratie verbeteren
if(UNIX)
    # Op Ubuntu: gebruik FetchContent om Google Test te downloaden en bouwen
    include(FetchContent)
    FetchContent_Declare(
            googletest
            GIT_REPOSITORY https://github.com/google/googletest.git
            GIT_TAG release-1.12.1
    )
    FetchContent_MakeAvailable(googletest)

    # Gebruik de opgebouwde target in plaats van handmatig te linken
    set(GTEST_LIBRARIES gtest gtest_main)
else()
    # Voor Windows: gebruik de bestaande methode
    include_directories(gtest/include)
    link_directories(gtest/lib)
    set(GTEST_LIBRARIES gtest)
endif()

# Verdere configuratie blijft hetzelfde
set(RELEASE_SOURCE_FILES
        src/baan.cpp
        src/situatie.cpp
        src/verkeerslicht.cpp
        src/voertuig.cpp
        src/main_release.cpp
        src/tinyxml.cpp
        src/tinystr.cpp
        src/tinyxmlerror.cpp
        src/tinyxmlparser.cpp
        src/simulatie.cpp
)

set(DEBUG_SOURCE_FILES
        src/baan.cpp
        src/situatie.cpp
        src/verkeerslicht.cpp
        src/voertuig.cpp
        src/main_debug.cpp
        src/tinyxml.cpp
        src/tinystr.cpp
        src/tinyxmlerror.cpp
        src/tinyxmlparser.cpp
        src/simulatie.cpp
)

# Create RELEASE target
add_executable(release_target ${RELEASE_SOURCE_FILES})

# Create DEBUG target
add_executable(debug_target ${DEBUG_SOURCE_FILES})

# Link library (aangepast voor platformonafhankelijkheid)
target_link_libraries(debug_target ${GTEST_LIBRARIES})