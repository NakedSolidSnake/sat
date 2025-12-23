add_library (sat_directory "")

target_sources (sat_directory
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_directory.c
)

target_include_directories (sat_directory
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_directory
    PUBLIC
    sat_linked_list
)

install (FILES include/sat_directory.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_directory.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_directory")