add_executable (sat_udp_server_sample ${CMAKE_CURRENT_LIST_DIR}/sat_udp_server_sample.c)
target_link_libraries (sat_udp_server_sample PUBLIC sat)
set_target_properties (sat_udp_server_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples)

add_executable (sat_udp_client_sample ${CMAKE_CURRENT_LIST_DIR}/sat_udp_client_sample.c)
target_link_libraries (sat_udp_client_sample PUBLIC sat)
set_target_properties (sat_udp_client_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples)

add_executable (sat_udp_multicast_sample ${CMAKE_CURRENT_LIST_DIR}/sat_udp_multicast_sample.c)
target_link_libraries (sat_udp_multicast_sample PUBLIC sat)
set_target_properties (sat_udp_multicast_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples)