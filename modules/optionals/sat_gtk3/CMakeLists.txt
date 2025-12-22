if (${SAT_GTK3})

    message (STATUS "SAT_GTK3 support enable")

    check_library (GTK "gtk+-3.0")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()