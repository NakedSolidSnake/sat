option (SAT_GTK3 "SAT_GTK3 support" OFF)

if (${SAT_GTK3})
    message (STATUS "SAT_GTK3 support enable")

    check_library (GTK "gtk+-3.0")

    # if (GTK_FOUND)
    #     message (STATUS "GTK library Found")
    # else ()
    #     message (FATAL_ERROR "GTK library not found")
    # endif ()

    list (APPEND SAT_SOURCES 
        ${CMAKE_SOURCE_DIR}/src/sat_gtk3.c
        ${CMAKE_SOURCE_DIR}/src/sat_gtk3/sat_gtk3_window.c
        ${CMAKE_SOURCE_DIR}/src/sat_gtk3/sat_gtk3_button.c
        ${CMAKE_SOURCE_DIR}/src/sat_gtk3/sat_gtk3_label.c
        ${CMAKE_SOURCE_DIR}/src/sat_gtk3/sat_gtk3_box.c)
    
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_gtk3.h>\n")

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_gtk3)

    list (APPEND SAT_LIBRARIES ${GTK_LIBRARIES})

    add_definitions (${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})

    install (FILES include/sat_gtk3.h                    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_gtk3/sat_gtk3_box.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_gtk3/sat_gtk3_button.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_gtk3/sat_gtk3_label.h     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_gtk3/sat_gtk3_widget.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_gtk3/sat_gtk3_window.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

endif ()