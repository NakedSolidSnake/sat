if (${SAT_PROPERTIES})

    message (STATUS "SAT_PROPERTIES support enable")
    
    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)    
endif ()
