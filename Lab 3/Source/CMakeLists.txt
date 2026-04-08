cmake_minimum_required(VERSION 3.10)
project( 21127469 )

set (CMAKE_CXX_STANDARD 20)

file(GLOB SOURCES "src/*.cpp")

set(OpenCV_DIR "C:/opencv/build/x64/vc16/lib")
find_package(OpenCV REQUIRED)
include_directories( ${OpenCV_INCLUDE_DIRS} )

add_executable( 21127469 ${SOURCES})

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT 21127469)

target_link_libraries( 21127469 ${OpenCV_LIBS} )
target_include_directories(21127469 PRIVATE ${PROJECT_SOURCE_DIR}/includes)