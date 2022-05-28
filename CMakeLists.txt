cmake_minimum_required(VERSION 3.22)
project(zadanie_rekrytacyjne C)

set(CMAKE_C_STANDARD 99)
set(THREADS_PREFER_PTHREAD_FLAG ON)


add_compile_options(-Wall -Wextra -pedantic -Werror)

add_subdirectory(src)
add_subdirectory(tests)

enable_testing()

add_test(NAME test_queue COMMAND test_queue)
add_test(NAME test_reader COMMAND test_reader)

