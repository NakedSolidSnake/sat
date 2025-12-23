add_library (sat_worker "")

target_sources (sat_worker
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_worker.c
)

target_include_directories (sat_worker
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_worker
    PUBLIC
    sat_queue
)

install (FILES include/sat_worker.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_worker.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_worker")