option (SAT_PROPERTIES "SAT_PROPERTIES support" OFF)

if (${SAT_PROPERTIES})
    message (STATUS "SAT_PROPERTIES support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_properties.c
                             ${CMAKE_SOURCE_DIR}/src/sat_properties/ini.c)

    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_properties.h>\n")

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_properties)

    install (FILES include/sat_properties.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()