add_library (sat_scheduler "")

target_sources (sat_scheduler
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_scheduler.c
)

target_include_directories (sat_scheduler
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_scheduler
    PUBLIC
    sat_set
    sat_time
    pthread
)

install (FILES include/sat_scheduler.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_scheduler")