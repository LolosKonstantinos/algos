add_subdirectory(double-linked)
add_subdirectory(simple-linked)

add_library(algos-linked-lists SHARED double-linked/double-linked-list.c simple-linked/linked-list.c)
target_link_libraries(algos-linked-lists PRIVATE lib_includes)