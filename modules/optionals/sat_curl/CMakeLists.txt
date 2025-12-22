if (${SAT_CURL})

    message (STATUS "SAT_CURL support enable")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)
    
endif ()

