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
