add_library (sat_array "")

target_sources (sat_array
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_array.c
)

target_include_directories (sat_array
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_array
    PUBLIC
    sat_iterator
)

install (FILES include/sat_array.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})