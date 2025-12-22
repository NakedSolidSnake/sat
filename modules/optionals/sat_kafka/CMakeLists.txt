if (${SAT_KAFKA})

    message (STATUS "SAT_KAFKA support enable")

    check_library (GLIB "glib-2.0")
    check_library (RDKAFKA "rdkafka")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
