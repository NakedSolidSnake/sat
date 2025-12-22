if (${SAT_LOKI})

    message (STATUS "SAT_LOKI support enable")

    if (NOT SAT_CURL)
        message (FATAL_ERROR "SAT_LOKI requires SAT_CURL to be enabled. Please enable SAT_CURL first.")
    endif()

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)
    

endif ()


