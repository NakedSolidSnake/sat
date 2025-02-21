add_executable (sat_process_sample ${CMAKE_CURRENT_LIST_DIR}/sat_process_sample.c)
target_link_libraries (sat_process_sample PUBLIC sat)
set_target_properties (sat_process_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/process)

add_executable (hello ${CMAKE_CURRENT_LIST_DIR}/hello.c)
set_target_properties (hello PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/process)

add_executable (hello_args ${CMAKE_CURRENT_LIST_DIR}/hello_args.c)
set_target_properties (hello_args PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/process)

add_executable (critical ${CMAKE_CURRENT_LIST_DIR}/critical.c)
set_target_properties (critical PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/process)

add_executable (sat_process_monitor_sample ${CMAKE_CURRENT_LIST_DIR}/sat_process_monitor_sample.c)
target_link_libraries (sat_process_monitor_sample PUBLIC sat)
set_target_properties (sat_process_monitor_sample PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/process)