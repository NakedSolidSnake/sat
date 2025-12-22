if (${SAT_GTK3})

    check_library (GTK "gtk+-3.0")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()