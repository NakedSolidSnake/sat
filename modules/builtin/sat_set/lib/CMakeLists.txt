add_library (sat_set "")

target_sources (sat_set
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_set.c
)

target_include_directories (sat_set
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_set
    PUBLIC
    sat_array
)

install (FILES include/sat_set.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
