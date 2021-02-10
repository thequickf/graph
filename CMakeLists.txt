cmake_minimum_required(VERSION 3.10.0)

set(CMAKE_CXX_STANDARD 20)
project(graph)

option(BUILD_TESTS "Build graph tests." TRUE)

ADD_SUBDIRECTORY(src)

if(BUILD_TESTS)
  enable_testing()
  ADD_SUBDIRECTORY(third_party/googletest)
  ADD_SUBDIRECTORY(tests)
endif(BUILD_TESTS)
