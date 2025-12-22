if (${SAT_REDIS})

    message (STATUS "SAT_REDIS support enable")
    
    check_library (REDIS "hiredis")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()