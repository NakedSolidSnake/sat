add_library (sat_log SHARED "")

target_sources (sat_log
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_log.c
)

target_include_directories (sat_log
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_log
    PUBLIC
    sat_status
)

install (FILES include/sat_log.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
