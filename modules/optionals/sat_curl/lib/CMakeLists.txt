add_library (sat_curl "")

target_sources (sat_curl
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_curl.c
)

target_include_directories (sat_curl
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_curl
    PUBLIC
    sat_status
    curl
)

install (FILES include/sat_curl.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_curl.h>\n")

set_property (GLOBAL PROPERTY SAT_CURL_COMPILED TRUE)

