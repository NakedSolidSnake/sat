add_library (sat_linked_list "")

target_sources (sat_linked_list
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_linked_list.c
)

target_include_directories (sat_linked_list
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_linked_list
    PUBLIC
    sat_iterator
)

install (FILES include/sat_linked_list.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_linked_list.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_linked_list")