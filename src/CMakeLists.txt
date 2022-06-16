macro(make_library target source)
    add_library(${target} ${source})
    target_include_directories(${target} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
endmacro(make_library)

make_library(queue queue.c)
make_library(reader reader.c)
make_library(analyzer analyzer.c)
make_library(printer printer.c)
make_library(logger logger.c)
make_library(watchdog watchdog.c)

find_package(Threads REQUIRED)


add_executable(main main.c main.h)
target_link_libraries(main queue reader analyzer printer logger watchdog)
target_link_libraries(main Threads::Threads)
target_link_libraries(main m)