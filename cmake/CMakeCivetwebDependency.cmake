option (SAT_WEBSERVER "SAT_WEBSERVER support" OFF)

if (${SAT_WEBSERVER})
    message (STATUS "SAT_WEBSERVER support enable")

    check_library (CIVETWEB "civetweb")
    # check_library (DL "dl")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_webserver.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_webserver.h>\n")
    list (APPEND SAT_LIBRARIES civetweb dl pthread)

    install (FILES include/sat_webserver.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()