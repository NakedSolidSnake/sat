if (${SAT_SQLITE})
    message (STATUS "SAT_SQLITE support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_sqlite.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_sqlite.h>\n")
    list (APPEND SAT_LIBRARIES sqlite3)
endif ()