add_library (sat_file "")

target_sources (sat_file
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_file.c
)

target_include_directories (sat_file
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_file
    PUBLIC
    sat_status
)

install (FILES include/sat_file.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
