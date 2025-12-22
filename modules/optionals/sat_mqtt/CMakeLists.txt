if (${SAT_MQTT})

    message (STATUS "SAT_MQTT support enable")
    
    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)
    
endif ()


