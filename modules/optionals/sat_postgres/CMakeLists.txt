if (${SAT_POSTGRES})

    message (STATUS "SAT_POSTGRES support enable")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)
    
endif ()