cmake_minimum_required(VERSION 3.20)
project(lmem VERSION 1.0.0.1 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)

add_executable(main main.cc
                    global_var.hpp 
                    free_list.cc
                    size_class.cc
                    thread_cache.cc
                    span_list.cc
                    central_cache.cc
                    page_cache.cc
                    sys_alloc.cc 
                    concurrent_allocate.cc
                    )


target_compile_options(main PRIVATE -pthread)

target_link_libraries(main PRIVATE -pthread)