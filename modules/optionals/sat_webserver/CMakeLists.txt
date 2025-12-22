if (${SAT_WEBSERVER})

    message (STATUS "SAT_WEBSERVER support enable")
    
    check_library (CIVETWEB "civetweb")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
