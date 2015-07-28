set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
add_library(tikhonovSVDCore TikhonovSVD.cpp)
add_subdirectory(unit_test)
target_link_libraries(tikhonovSVDCore ${gsl_LIBRARIES})
