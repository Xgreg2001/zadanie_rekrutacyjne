macro(make_library target source)
    add_library(${target} ${source})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(make_library)

make_library(queue queue.c)

add_executable(main main.c)
target_link_libraries(main queue)