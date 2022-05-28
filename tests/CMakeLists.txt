macro(make_test target source library)
    add_executable(${target} ${source})
    target_link_libraries(${target} ${library})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(make_test)

make_test(test_queue queue_test.c queue)
make_test(test_reader reader_test.c reader)