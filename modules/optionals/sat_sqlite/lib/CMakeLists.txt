add_library (sat_sqlite "")

target_sources (sat_sqlite
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_sqlite.c
)

target_include_directories (sat_sqlite
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_sqlite
    PUBLIC
    sat_status
    sqlite3
)

install (FILES include/sat_sqlite.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_sqlite.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_sqlite")