add_executable (test_sat_uuid ${CMAKE_CURRENT_LIST_DIR}/test_sat_uuid.c)
target_link_libraries (test_sat_uuid PUBLIC sat)
add_test (NAME test_sat_uuid COMMAND test_sat_uuid)

add_executable (test_sat_file ${CMAKE_CURRENT_LIST_DIR}/test_sat_file.c)
target_link_libraries (test_sat_file PUBLIC sat)
add_test (NAME test_sat_file COMMAND test_sat_file)

add_executable (test_sat_time ${CMAKE_CURRENT_LIST_DIR}/test_sat_time.c)
target_link_libraries (test_sat_time PUBLIC sat)
add_test (NAME test_sat_time COMMAND test_sat_time)