option (SAT_CURL "SAT_CURL support" OFF)

if (${SAT_CURL})
    message (STATUS "SAT_CURL support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_curl.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_curl.h>\n")
    list (APPEND SAT_LIBRARIES curl)

    install (FILES include/sat_curl.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()