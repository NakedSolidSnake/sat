option (SAT_REDIS "SAT_REDIS support" OFF)

if (${SAT_REDIS})
    message (STATUS "SAT_REDIS support enable")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_redis.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_redis.h>\n")
    list (APPEND SAT_LIBRARIES hiredis)

    install (FILES include/sat_redis.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()