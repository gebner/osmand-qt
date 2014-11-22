project(OsmAnd)

find_package(GDAL REQUIRED)
find_package(ICU REQUIRED)
find_package(EXPAT REQUIRED)
#find_package(GIF REQUIRED)
find_package(JPEG REQUIRED)
find_package(PNG REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Boost REQUIRED)
find_package(GLEW REQUIRED)
find_package(Freetype REQUIRED)
find_package(LZO REQUIRED)

add_library(gdal_static empty.cc)
target_link_libraries(gdal_static ${GDAL_LIBRARY})
include_directories(${GDAL_INCLUDE_DIR})

add_library(icu4c_static empty.cc)
target_link_libraries(icu4c_static ${ICU_LIBRARIES})
include_directories(${ICU_INCLUDE_DIRS})

add_library(expat_static empty.cc)
target_link_libraries(expat_static ${EXPAT_LIBRARIES})
include_directories(${EXPAT_INCLUDE_DIRS})

#add_library(gif_static empty.cc)
#target_link_libraries(gif_static ${GIF_LIBRARIES})
#include_directories(${GIF_INCLUDE_DIR})

add_library(jpeg_static empty.cc)
target_link_libraries(jpeg_static ${JPEG_LIBRARIES})
include_directories(${JPEG_INCLUDE_DIR})

add_library(png_static empty.cc)
target_link_libraries(png_static ${PNG_LIBRARIES})
include_directories(${PNG_INCLUDE_DIRS})

add_library(z_static empty.cc)
target_link_libraries(z_static ${ZLIB_LIBRARIES})
include_directories(${ZLIB_INCLUDE_DIRS})

add_library(boost_static_precompiled empty.cc)
target_link_libraries(boost_static_precompiled ${BOOST_LIBRARIES})
include_directories(${BOOST_INCLUDE_DIRS})

add_library(glew_static empty.cc)
target_link_libraries(glew_static ${GLEW_LIBRARIES})
include_directories(${GLEW_INCLUDE_DIRS})

add_library(freetype2_static empty.cc)
target_link_libraries(freetype2_static ${FREETYPE_LIBRARIES})
include_directories(${FREETYPE_INCLUDE_DIRS})

set(OSMAND_ROOT ${PROJECT_SOURCE_DIR})
set(CMAKE_SHARED_LIBS_ALLOWED_ON_TARGET ON)
set(CMAKE_TARGET_OS "linux")
set(CMAKE_TARGET_CPU_ARCH_FAMILY "x86")
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fPIC -mssse3 -std=gnu++11")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -fPIC -mssse3")

include(CheckFunctionExists)
include(CheckCSourceCompiles)
include(build/utils/common.cmake)

# osmand needs an old version of protobuf
add_subdirectory(core/externals/protobuf)

# osmand needs an old version of glm
add_subdirectory(core/externals/glm)

# osmand includes old skia version which does not work with giflib-5
add_subdirectory(core/externals/giflib)

# skia is usually not packaged
add_subdirectory(core/externals/skia)

# osmand includes libarchive/archive.h instead of archive.h
include_directories(${LZO_INCLUDE_DIRS})
link_libraries(${LZO_LIBRARIES})
add_subdirectory(core/externals/libarchive)

add_subdirectory(core)
include_directories(core/include)

# prevent resources from being built every single time because our
# resources doesn't have a git dir
include(ExternalProject)
ExternalProject_Add(resources
  SOURCE_DIR ${PROJECT_SOURCE_DIR}/resources
  CMAKE_ARGS -DCMAKE_SHARED_LIBS_ALLOWED_ON_TARGET=ON
  INSTALL_COMMAND ""
  )
ExternalProject_Get_Property(resources BINARY_DIR)
add_library(OsmAndCore_ResourcesBundle_shared empty.cc)
add_dependencies(OsmAndCore_ResourcesBundle_shared resources)
target_link_libraries(OsmAndCore_ResourcesBundle_shared
  ${BINARY_DIR}/libOsmAndCore_ResourcesBundle_shared.so)
