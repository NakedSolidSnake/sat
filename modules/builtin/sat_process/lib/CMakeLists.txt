add_library (sat_process "")

target_sources (sat_process
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_process.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_process_monitor.c
)

target_include_directories (sat_process
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_process
    PUBLIC
    sat_array
)

install (FILES include/sat_process.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_process_monitor.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
