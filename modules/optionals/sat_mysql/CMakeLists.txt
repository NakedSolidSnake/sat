if (${SAT_MYSQL})

    message (STATUS "SAT_MYSQL support enable")
    
    check_library (MYSQL "mysqlclient")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
