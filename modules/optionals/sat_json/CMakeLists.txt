if (${SAT_JSON})

    message (STATUS "SAT_JSON support enable")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()

