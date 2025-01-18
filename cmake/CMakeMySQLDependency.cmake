option (SAT_MYSQL "SAT_MYSQL support" OFF)

if (${SAT_MYSQL})
    message (STATUS "SAT_MYSQL support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_mysql.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_mysql.h>\n")
    list (APPEND SAT_LIBRARIES mysqlclient)

    install (FILES include/sat_mysql.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()