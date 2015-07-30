cmake_minimum_required(VERSION 3.1)
project(InverseProblem)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake/Modules)
include(gmock)

set(SOURCE_FILES main.cpp)
add_executable(InverseProblem ${SOURCE_FILES})

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

find_package( PkgConfig REQUIRED)
pkg_check_modules( gsl REQUIRED gsl )
include_directories(${gsl_INCLUDE_DIRS})
target_link_libraries(InverseProblem ${gsl_LIBRARIES})


add_subdirectory(GeneticAlgorithmCore)
add_subdirectory(TikhonovSVDCore)
add_subdirectory(VisualCore)

target_link_libraries(InverseProblem geneticAlgorithmCore)
target_link_libraries(InverseProblem tikhonovSVDCore)
target_link_libraries(InverseProblem visualCore)
