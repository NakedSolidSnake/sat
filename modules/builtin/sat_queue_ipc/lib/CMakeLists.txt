add_library (sat_queue_ipc "")

target_sources (sat_queue_ipc
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue_ipc.c
)

target_include_directories (sat_queue_ipc
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_queue_ipc
    PUBLIC
    sat_status
)

install (FILES include/sat_queue_ipc.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_queue_ipc.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_queue_ipc")