add_library (sat_stack "")

target_sources (sat_stack
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_stack.c
)

target_include_directories (sat_stack
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_stack
    PUBLIC
    sat_status
)

install (FILES include/sat_stack.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_stack")