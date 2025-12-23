add_library (sat_iterator "")

target_sources (sat_iterator
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_iterator.c
)

target_include_directories (sat_iterator
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_iterator
    PUBLIC
    sat_status
)

install (FILES include/sat_iterator.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_iterator.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_iterator")