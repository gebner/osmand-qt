project(OsmAndQt)
cmake_minimum_required(VERSION 3.0)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

list(APPEND CMAKE_MODULE_PATH "${PROJECT_SOURCE_DIR}/cmake")

add_subdirectory(osmand)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=gnu++1y")

set(CMAKE_AUTOMOC ON)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Qml REQUIRED)
find_package(Qt5Quick REQUIRED)

qt5_add_resources(RESOURCES resources.qrc)

add_executable(main
  src/main.cpp
  src/map.h src/map.cpp
  src/resources.h src/resources.cpp
  ${RESOURCES}
)
target_link_libraries(main
  Qt5::Widgets Qt5::Qml Qt5::Quick
  OsmAndCore_shared OsmAndCore_ResourcesBundle)
