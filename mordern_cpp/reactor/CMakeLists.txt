# Minimum CMake version
cmake_minimum_required(VERSION 3.10)

# Project name
project(EchoServer)

# Set the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# 定义变量以指定 libosmium 的路径，默认值为空
set(LIBOSMIUM_DIR "" CACHE PATH "Path to libosmium directory")

# 如果没有设置路径，则提示错误
if(NOT LIBOSMIUM_DIR)
    message(FATAL_ERROR "Please specify the path to libosmium using -DLIBOSMIUM_DIR=/path/to/libosmium")
endif()

# 定义变量以指定 libmapnik 的路径，默认值为空
set(LIBMAPNIK_DIR "" CACHE PATH "Path to libmapnik directory")

# 如果没有设置路径，则提示错误
if(NOT LIBMAPNIK_DIR)
    message(FATAL_ERROR "Please specify the path to libosmium using -LIBMAPNIK_DIR=/path/to/libosmium")
endif()

# Include directories
include_directories(include ${LIBOSMIUM_DIR}/include 
                    ${LIBMAPNIK_DIR}/include 
                    ${LIBMAPNIK_DIR}/deps/mapbox/variant/include
                    ${LIBMAPNIK_DIR}/deps/mapbox/geometry/include
                    ${LIBMAPNIK_DIR}/deps/agg/include
                    ${LIBMAPNIK_DIR}/deps/
                    ${LIBMAPNIK_DIR}/deps/mapbox/polylabel/include
                    ${LIBMAPNIK_DIR})

# Find dependecy for curl
find_package(CURL REQUIRED)
find_package(Boost REQUIRED COMPONENTS regex)
find_package(Threads REQUIRED)
find_package(ZLIB REQUIRED)
find_package(Protobuf REQUIRED)
find_package(BZip2 REQUIRED)
find_package(EXPAT REQUIRED)
find_package(Freetype REQUIRED)
find_package(PNG REQUIRED)
find_package(PkgConfig REQUIRED)
find_package(ICU REQUIRED COMPONENTS uc i18n)
pkg_check_modules(HARFBUZZ REQUIRED harfbuzz)

# 查找 JPEG 库
find_package(JPEG REQUIRED)

# 查找 TIFF 库
find_package(TIFF REQUIRED)

# 查找 WebP 库
find_library(WEBP_LIB webp PATHS /usr/lib /usr/local/lib)
find_path(WEBP_INCLUDE_DIR webp/decode.h PATHS /usr/include /usr/local/include)
include_directories(${WEBP_INCLUDE_DIR})

include_directories(${HARFBUZZ_INCLUDE_DIRS})
link_directories(${HARFBUZZ_LIBRARY_DIRS})

# 添加 Expat 的头文件路径
include_directories(${EXPAT_INCLUDE_DIRS})

# 添加 bzip2 的头文件路径
include_directories(${BZIP2_INCLUDE_DIR})

# 添加 Freetype 的头文件路径
include_directories(${FREETYPE_INCLUDE_DIRS})

if(NOT TARGET Protobuf::Protobuf)
    add_library(Protobuf::Protobuf INTERFACE IMPORTED)
    set_target_properties(Protobuf::Protobuf PROPERTIES
        INTERFACE_INCLUDE_DIRECTORIES "${Protobuf_INCLUDE_DIRS}"
        INTERFACE_LINK_LIBRARIES "${Protobuf_LIBRARIES}")
endif()

file(GLOB_RECURSE MAPNIK_SOURCES ${LIBMAPNIK_DIR}/src/*.cpp)
file(GLOB_RECURSE AGG_SOURCES ${LIBMAPNIK_DIR}/deps/agg/src/*.cpp)

# Source files
set(SRC_FILES
    src/main_server.cc        # Entry point
    src/Acceptor.cc
    src/Epoller.cc
    src/Handler.cc
    src/Reactor.cc
    src/Server.cc
    ${MAPNIK_SOURCES}
    ${AGG_SOURCES}
)

# Create the executable
add_executable(EchoServer ${SRC_FILES})
add_executable(EchoClient src/main_client.cc)
add_executable(server_simple src/main_server_simple.cc)
add_executable(osm_downloader utility/openstreetmap/src/download_osm.cc)
add_executable(osm_parser utility/openstreetmap/src/parse_osm.cc)
# Link libraries (if needed, e.g., pthread for multithreading)
target_link_libraries(EchoServer pthread ${FREETYPE_LIBRARIES} 
                        ${EXPAT_LIBRARIES} 
                        ${Boost_LIBRARIES} 
                        ${BZIP2_LIBRARIES}
                        ${HARFBUZZ_LIBRARIES}
                        ${WEBP_LIB}
                        Threads::Threads 
                        ZLIB::ZLIB 
                        Protobuf::Protobuf
                        PNG::PNG
                        ICU::uc
                        ICU::i18n
                        Boost::regex
                        JPEG::JPEG
                        TIFF::TIFF)
target_link_libraries(osm_downloader CURL::libcurl)
target_link_libraries(osm_parser 
                        ${FREETYPE_LIBRARIES} 
                        ${EXPAT_LIBRARIES} 
                        ${Boost_LIBRARIES} 
                        ${BZIP2_LIBRARIES} 
                        Threads::Threads ZLIB::ZLIB Protobuf::Protobuf)