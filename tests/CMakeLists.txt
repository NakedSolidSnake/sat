add_executable (test_sat_uuid ${CMAKE_CURRENT_LIST_DIR}/test_sat_uuid.c)
target_link_libraries (test_sat_uuid PUBLIC sat)
add_test (NAME test_sat_uuid COMMAND test_sat_uuid)