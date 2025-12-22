add_library (sat_loki "")

target_sources (sat_loki
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_loki.c
)

target_include_directories (sat_loki
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_loki
    PUBLIC
    sat_time
    sat_curl
)

install (FILES include/sat_loki.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_loki.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_loki")