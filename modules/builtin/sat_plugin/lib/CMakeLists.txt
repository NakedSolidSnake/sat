add_library (sat_plugin "")

target_sources (sat_plugin
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_plugin.c
)

target_include_directories (sat_plugin
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_plugin
    PUBLIC
    sat_status
)

install (FILES include/sat_plugin.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_plugin.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_plugin")