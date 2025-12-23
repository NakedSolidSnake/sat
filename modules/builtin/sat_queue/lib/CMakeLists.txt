add_library (sat_queue "")

target_sources (sat_queue
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue.c
)

target_include_directories (sat_queue
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_queue
    PUBLIC
    sat_status
)

install (FILES include/sat_queue.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_queue")