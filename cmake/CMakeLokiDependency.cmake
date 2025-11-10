option (SAT_LOKI "SAT_LOKI support" OFF)

if (${SAT_LOKI})
    message (STATUS "SAT_LOKI support enable")

    if (NOT SAT_CURL)
        message (FATAL_ERROR "SAT_LOKI requires SAT_CURL to be enabled. Please enable SAT_CURL first.")
    endif()

    # check_library (CIVETWEB "civetweb")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_loki.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_loki.h>\n")

    install (FILES include/sat_loki.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()