project(OsmAndQt)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -std=gnu++11")

add_subdirectory(osmand)

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)

add_executable(main main.cpp)
target_link_libraries(main
  Qt5::Widgets
  OsmAndCore_shared OsmAndCore_ResourcesBundle)
