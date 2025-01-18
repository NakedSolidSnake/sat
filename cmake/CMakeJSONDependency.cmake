option (SAT_JSON "SAT_JSON support" OFF)

if (${SAT_JSON})
    message (STATUS "SAT_JSON support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_json.c
                             ${CMAKE_SOURCE_DIR}/src/sat_json/cJSON_Utils.c
                             ${CMAKE_SOURCE_DIR}/src/sat_json/cJSON.c
    )

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_json)

    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_json.h>\n")

    install (FILES include/sat_json.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()