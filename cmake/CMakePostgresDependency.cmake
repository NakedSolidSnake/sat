option (SAT_POSTGRES "SAT_POSTGRES support" OFF)

if (${SAT_POSTGRES})
    message (STATUS "SAT_POSTGRES support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_postgres.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_postgres.h>\n")
    list (APPEND SAT_LIBRARIES pq)
    list (APPEND SAT_INCLUDES /usr/include/postgresql)

    install (FILES include/sat_postgres.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()