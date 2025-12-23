add_library (sat_shared_memory "")

target_sources (sat_shared_memory
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_shared_memory.c
)

target_include_directories (sat_shared_memory
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_shared_memory
    PUBLIC
    sat_status
)

install (FILES include/sat_shared_memory.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_shared_memory.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_shared_memory")