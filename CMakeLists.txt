cmake_minimum_required(VERSION 3.16)
project(zadanie_rekrytacyjne C)

set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)

if (CMAKE_C_COMPILER_ID MATCHES GNU)
    set(CMAKE_C_FLAGS "-Wall -pedantic -Wextra")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
endif ()

if (CMAKE_C_COMPILER_ID MATCHES Clang)
    set(CMAKE_C_FLAGS "-Weverything -Wno-disabled-macro-expansion -Wno-vla -Wno-padded -Wno-missing-variable-declarations -Wno-unused-parameter")
    set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS} -g")
    set(CMAKE_C_FLAGS_RELEASE "${CMAKE_C_FLAGS} -O3")
endif ()

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

add_test(NAME test_queue COMMAND test_queue)

