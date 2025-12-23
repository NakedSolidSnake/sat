add_library (sat_udp "")

target_sources (sat_udp
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server_abstract.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server_interactive.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server_async.c
)

target_include_directories (sat_udp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_udp
    PUBLIC
    sat_status
)

install (FILES include/public/sat_udp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/public/sat_udp_types.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})