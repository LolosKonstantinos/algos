add_library(lib_includes INTERFACE
        memory_pool.h
        queue.h
        stack.h
        hash_table.h
        hash-functions.h
        linked-list.h
        double-linked-list.h
        vector.h
        binary-trees.h
)

target_include_directories(lib_includes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})