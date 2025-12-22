add_library (sat_mysql "")

target_sources (sat_mysql
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_mysql.c
)

target_include_directories (sat_mysql
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_mysql
    PUBLIC
    sat_status
    mysqlclient
)

install (FILES include/sat_mysql.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_mysql.h>\n")

set_property (GLOBAL PROPERTY SAT_MYSQL_COMPILED TRUE)