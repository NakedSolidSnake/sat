if (${SAT_MYSQL})
    check_library (MYSQL "mysqlclient")

    add_subdirectory (lib)
    add_subdirectory (samples)
    add_subdirectory (tests)
    add_subdirectory (manpages)

endif ()
