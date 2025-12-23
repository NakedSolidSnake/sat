add_library (sat_process "")

target_sources (sat_process
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_process.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_process_monitor.c
)

target_include_directories (sat_process
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_process
    PUBLIC
    sat_array
)

install (FILES include/sat_process.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_process_monitor.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_process.h>\n")
file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_process_monitor.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_process")