cmake_minimum_required(VERSION 3.10)

project(PDFtoImageConverter)

set(CMAKE_CXX_STANDARD 14)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER_CPP REQUIRED poppler-cpp)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
include_directories(${POPPLER_CPP_INCLUDE_DIRS})

add_executable(PDFtoImageConverter src/main.cpp)

target_link_libraries(PDFtoImageConverter ${Boost_LIBRARIES} ${OpenCV_LIBS} poppler-cpp)
