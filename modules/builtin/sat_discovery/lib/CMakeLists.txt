add_library (sat_discovery "")

target_sources (sat_discovery
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_interest.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_frame.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_node.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_announce.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_heartbeat.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_interest.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_node_ageing.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_scan.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_handle_frame_announce.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_handle_frame_heartbeat.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_handle_frame_interest.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_handle_frame_vanish.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_discovery_service_vanish.c
)

target_include_directories (sat_discovery
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_discovery
    PUBLIC
    sat_udp
    sat_scheduler
    sat_uuid
    sat_log
    sat_network
)

install (FILES include/sat_discovery.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_discovery")