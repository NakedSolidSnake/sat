add_library (sat_webserver "")

target_sources (sat_webserver
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_webserver.c
)

target_include_directories (sat_webserver
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_webserver
    PUBLIC
    sat_array
    civetweb
    dl
    pthread
)

install (FILES include/sat_webserver.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_webserver.h>\n")
set_property (GLOBAL PROPERTY SAT_WEBSERVER_COMPILED TRUE)
