if (${SAT_JSON})
    add_executable (test_sat_json ${CMAKE_CURRENT_LIST_DIR}/test_sat_json.c)
    target_link_libraries (test_sat_json PUBLIC sat)
    add_test (NAME test_sat_sqlite COMMAND test_sat_json ${CMAKE_CURRENT_LIST_DIR})
endif ()