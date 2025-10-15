cmake_minimum_required(VERSION 3.31)
project(algos C)

set(CMAKE_C_STANDARD 17)

add_subdirectory(include)
add_subdirectory(src)

add_library(${CMAKE_PROJECT_NAME} SHARED
        src/hash-table/hash_table.c
        src/linked-lists/simple-linked/linked-list.c
        src/linked-lists/double-linked/double-linked-list.c
        src/memory-pool/memory_pool.c
        src/queue/queue.c
        src/stack/stack.c
        src/vector/vector.c
        src/binary-trees/binary-trees.c
)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC lib_includes)
