cmake_minimum_required(VERSION 3.7)
project(descore)
include_directories(${PROJECT_SOURCE_DIR}/include)
aux_source_directory(src DIR_SRCS)
add_library(descore SHARED
    ${DIR_SRCS})