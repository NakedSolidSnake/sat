list (APPEND SAT_BUILTIN_LIBRARIES 
             uuid
             m
             pthread
             dl
             sat_status
             sat_array
             sat_iterator
             sat_cache
             sat_plugin
             sat_uuid
             sat_queue
             sat_linked_list
             sat_directory
             sat_set
             sat_event
             sat_channel
             sat_udp
             sat_network
             sat_worker
             sat_time
             sat_stack
             sat_process
             sat_file
             sat_validation
             sat_log
             sat_map
             sat_tcp
             sat_scheduler
             sat_shared_memory
             sat_discovery
             sat_queue_ipc
             )

get_property (SAT_OPTIONALS_LIBRARIES GLOBAL PROPERTY SAT_OPTIONAL_LIBS)

add_library (sat SHARED "")
target_sources (sat
    PRIVATE 
    ${CMAKE_CURRENT_LIST_DIR}/src/sat.c
)

target_include_directories (sat
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat
    PUBLIC
    ${SAT_BUILTIN_LIBRARIES}
    ${SAT_OPTIONALS_LIBRARIES}
)

install (FILES include/sat.h             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install (TARGETS sat LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})