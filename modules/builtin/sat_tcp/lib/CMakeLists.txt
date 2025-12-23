add_library (sat_tcp "")

target_sources (sat_tcp
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp_server_abstract.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp_server_interactive.c
)

target_include_directories (sat_tcp
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_tcp
    PUBLIC
    m
    sat_status
)

install (FILES include/public/sat_tcp.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/public/sat_tcp_types.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_tcp.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_tcp")
