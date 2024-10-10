cmake_minimum_required(VERSION 3.10)

project(test)

set(CMAKE_BUILD_TYPE "Debug")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/test/include)

set(GTEST_SRCS "")
file(GLOB_RECURSE GTEST_SRCS ${CMAKE_CURRENT_SOURCE_DIR}/test/*.cpp)
add_executable(${PROJECT_NAME} ${GTEST_SRCS})

target_link_libraries(${PROJECT_NAME} -lgtest)