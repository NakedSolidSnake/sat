add_library (sat_postgres "")

target_sources (sat_postgres
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_postgres.c
)

target_include_directories (sat_postgres
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_postgres
    PUBLIC
    sat_status
    pq
)

install (FILES include/sat_postgres.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_postgres.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_postgres")

list (APPEND SAT_INCLUDES /usr/include/postgresql)