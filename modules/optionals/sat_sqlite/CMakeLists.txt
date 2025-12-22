if (${SAT_SQLITE})
    message (STATUS "SAT_SQLITE support enable")

    check_library (SQLITE "sqlite3")
    
    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()


