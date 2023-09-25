cmake_minimum_required(VERSION 3.10)

project(PDFtoImageConverter)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)

find_package(Boost REQUIRED COMPONENTS filesystem)
find_package(OpenCV REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_check_modules(POPPLER_CPP REQUIRED poppler-cpp)
find_package(GTest REQUIRED)
# Google Test requires threading support
find_package(Threads REQUIRED)

include_directories(${Boost_INCLUDE_DIRS})
include_directories(${OpenCV_INCLUDE_DIRS})
link_directories(/opt/homebrew/opt/poppler/lib)
include_directories(${GTEST_INCLUDE_DIRS})

# Include the test subdirectory
add_subdirectory(tests)

add_executable(${PROJECT_NAME} src/main.cpp)

target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES} ${OpenCV_LIBS} poppler-cpp ${GTEST_LIBRARIES} Threads::Threads)
