cmake_minimum_required(VERSION 3.22.1)
project(soscore C)

add_library(
    soscore
    SHARED
    jni_bridge.c
    sos_engine.c
    geo_math.c
    sos_crypto.c
    sos_queue.c
)

find_library(log-lib log)
target_link_libraries(soscore ${log-lib} m)
