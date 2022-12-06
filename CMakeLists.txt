cmake_minimum_required(VERSION 3.19)
project(Temporal_path)

set(CMAKE_CXX_STANDARD 17)

aux_source_directory(core DIR)


add_executable(Temporal_path main.cpp ${DIR})
