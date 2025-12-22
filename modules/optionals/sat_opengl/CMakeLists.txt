if (${SAT_OPENGL})

    message (STATUS "SAT_OPENGL support enable")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
