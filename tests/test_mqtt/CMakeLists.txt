if (${SAT_MQTT})
    add_executable (test_sat_mqtt ${CMAKE_CURRENT_LIST_DIR}/test_sat_mqtt.c)
    target_link_libraries (test_sat_mqtt PUBLIC sat pthread)
    add_test (NAME test_sat_mqtt COMMAND test_sat_mqtt ${CMAKE_CURRENT_LIST_DIR})
endif ()