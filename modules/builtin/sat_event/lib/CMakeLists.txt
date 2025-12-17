add_library (sat_event "")

target_sources (sat_event
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_event.c
)

target_include_directories (sat_event
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_event
    PUBLIC
    sat_status
)

install (FILES include/sat_event.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
