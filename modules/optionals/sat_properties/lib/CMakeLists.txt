add_library (sat_properties "")

target_sources (sat_properties
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_properties.c
    ${CMAKE_CURRENT_LIST_DIR}/src/ini.c
)

target_include_directories (sat_properties
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include/public
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/include/private
)

target_link_libraries (sat_properties
    PUBLIC
    sat_status
)

install (FILES include/public/sat_properties.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_properties.h>\n")

set_property(GLOBAL PROPERTY SAT_PROPERTIES_COMPILED TRUE)