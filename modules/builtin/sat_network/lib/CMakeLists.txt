add_library (sat_network "")

target_sources (sat_network
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_network.c
)

target_include_directories (sat_network
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_network
    PUBLIC
    sat_array
)

install (FILES include/sat_network.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
