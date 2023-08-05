add_executable (test_sat_uuid ${CMAKE_CURRENT_LIST_DIR}/test_sat_uuid.c)
target_link_libraries (test_sat_uuid PUBLIC sat)
add_test (NAME test_sat_uuid COMMAND test_sat_uuid)

add_executable (test_sat_file ${CMAKE_CURRENT_LIST_DIR}/test_sat_file.c)
target_link_libraries (test_sat_file PUBLIC sat)
add_test (NAME test_sat_file COMMAND test_sat_file)

add_executable (test_sat_time ${CMAKE_CURRENT_LIST_DIR}/test_sat_time.c)
target_link_libraries (test_sat_time PUBLIC sat)
add_test (NAME test_sat_time COMMAND test_sat_time)

add_executable (test_sat_status ${CMAKE_CURRENT_LIST_DIR}/test_sat_status.c)
target_link_libraries (test_sat_status PUBLIC sat)
add_test (NAME test_sat_status COMMAND test_sat_status)

add_executable (test_sat_log ${CMAKE_CURRENT_LIST_DIR}/test_sat_log.c)
target_link_libraries (test_sat_log PUBLIC sat)
add_test (NAME test_sat_log COMMAND test_sat_log)

add_executable (test_sat_validation ${CMAKE_CURRENT_LIST_DIR}/test_sat_validation.c)
target_link_libraries (test_sat_validation PUBLIC sat)
add_test (NAME test_sat_validation COMMAND test_sat_validation)

add_executable (test_sat_event ${CMAKE_CURRENT_LIST_DIR}/test_sat_event.c)
target_link_libraries (test_sat_event PUBLIC sat)
add_test (NAME test_sat_event COMMAND test_sat_event)

add_executable (test_sat_stack ${CMAKE_CURRENT_LIST_DIR}/test_sat_stack.c)
target_link_libraries (test_sat_stack PUBLIC sat)
add_test (NAME test_sat_stack COMMAND test_sat_stack)