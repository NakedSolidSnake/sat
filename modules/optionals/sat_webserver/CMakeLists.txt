if (${SAT_WEBSERVER})

    check_library (CIVETWEB "civetweb")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
