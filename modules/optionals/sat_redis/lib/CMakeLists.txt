add_library (sat_redis "")

target_sources (sat_redis
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_redis.c
)

target_include_directories (sat_redis
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_redis
    PUBLIC
    sat_status
    "${REDIS_LIBRARIES}"
)

install (FILES include/sat_redis.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_redis.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_redis")