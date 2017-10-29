cmake_minimum_required(VERSION 3.5)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ../bin/)
add_compile_options(-std=c++11)
add_executable(main src/main.cpp src/CharCandidate.cpp src/CharCandidate.h src/io.cpp src/utils.h src/io.h src/search.cpp src/search.h)
