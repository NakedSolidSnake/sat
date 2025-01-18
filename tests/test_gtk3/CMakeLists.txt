if (${SAT_GTK3})
    add_executable (test_sat_gtk3_window ${CMAKE_CURRENT_LIST_DIR}/test_sat_gtk3_window.c)
    target_link_libraries (test_sat_gtk3_window PUBLIC sat)
    add_test (NAME test_sat_gtk3_window COMMAND test_sat_gtk3_window)

    add_executable (test_sat_gtk3_button ${CMAKE_CURRENT_LIST_DIR}/test_sat_gtk3_button.c)
    target_link_libraries (test_sat_gtk3_button PUBLIC sat)
    add_test (NAME test_sat_gtk3_button COMMAND test_sat_gtk3_button)

    add_executable (test_sat_gtk3_label ${CMAKE_CURRENT_LIST_DIR}/test_sat_gtk3_label.c)
    target_link_libraries (test_sat_gtk3_label PUBLIC sat)
    add_test (NAME test_sat_gtk3_label COMMAND test_sat_gtk3_label)

    add_executable (test_sat_gtk3_box ${CMAKE_CURRENT_LIST_DIR}/test_sat_gtk3_box.c)
    target_link_libraries (test_sat_gtk3_box PUBLIC sat)
    add_test (NAME test_sat_gtk3_box COMMAND test_sat_gtk3_box)

endif ()