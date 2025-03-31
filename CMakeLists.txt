cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_OSX_ARCHITECTURES "x86_64;arm64")
set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(GEODE_DISABLE_PRECOMPILED_HEADERS ON)

project(FakeRate VERSION 1.4.12)

add_library(${PROJECT_NAME} SHARED
    src/classes/FRDIBPopup.cpp
    src/classes/FREditPopup.cpp
    src/classes/FREffects.cpp
    src/classes/FRGDDPPopup.cpp
    src/classes/FRGRDPopup.cpp
    src/classes/FRSetDifficultyPopup.cpp
    src/classes/FRSetFeaturePopup.cpp
    src/classes/FRSetStarsPopup.cpp
    src/classes/TableNode.cpp
    src/hooks/LevelCell.cpp
    src/hooks/LevelInfoLayer.cpp
    src/FakeRate.cpp
)

if (NOT DEFINED ENV{GEODE_SDK})
    message(FATAL_ERROR "Unable to find Geode SDK! Please define GEODE_SDK environment variable to point to Geode")
else()
    message(STATUS "Found Geode: $ENV{GEODE_SDK}")
endif()

add_subdirectory($ENV{GEODE_SDK} ${CMAKE_CURRENT_BINARY_DIR}/geode)

setup_geode_mod(${PROJECT_NAME})
