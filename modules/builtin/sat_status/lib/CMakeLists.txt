add_library (sat_status "")

target_sources (sat_status
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_status.c
)

target_include_directories (sat_status
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

install (FILES include/sat_status.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_status")