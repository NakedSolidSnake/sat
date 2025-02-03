add_executable (sat_udp_server_sample ${CMAKE_CURRENT_LIST_DIR}/sat_udp_server_sample.c)
target_link_libraries (sat_udp_server_sample PUBLIC sat)

add_executable (sat_udp_client_sample ${CMAKE_CURRENT_LIST_DIR}/sat_udp_client_sample.c)
target_link_libraries (sat_udp_client_sample PUBLIC sat)
