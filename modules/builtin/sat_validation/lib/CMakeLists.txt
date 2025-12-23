add_library (sat_validation "")

target_sources (sat_validation
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_validation.c
)

target_include_directories (sat_validation
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_validation
    PUBLIC
    sat_status
)

install (FILES include/sat_validation.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_builtin.h "#include <sat_validation.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_validation")