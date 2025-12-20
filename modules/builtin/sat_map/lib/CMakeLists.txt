add_library (sat_map "")

target_sources (sat_map
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_map.c
)

target_include_directories (sat_map
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_map
    PUBLIC
    sat_array
)

install (FILES include/sat_map.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
