cmake_minimum_required(VERSION 3.22.1)
project(soscore C)

add_library(soscore SHARED sos_core.c)

find_library(log-lib log)
target_link_libraries(soscore ${log-lib})
