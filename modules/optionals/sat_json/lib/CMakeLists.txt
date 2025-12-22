add_library (sat_json "")

target_sources (sat_json
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_json.c
    ${CMAKE_CURRENT_LIST_DIR}/src/cJSON_Utils.c
    ${CMAKE_CURRENT_LIST_DIR}/src/cJSON.c
)

target_include_directories (sat_json
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_json
    PUBLIC
    sat_status
    m
)

install (FILES include/public/sat_json.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_json.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_json")