cmake_minimum_required(VERSION 3.5)

project(consistent_hashing)

set(CMAKE_BUILD_TYPE "Realse")

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

include_directories(include)

add_subdirectory(src)