cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

include (GNUInstallDirs)

enable_testing ()

list (APPEND SAT_SOURCES ${CMAKE_CURRENT_LIST_DIR}/src/sat_uuid.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_file.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_time.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_status.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_log.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_validation.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_event.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_stack.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_array.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp/sat_udp_server.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp/sat_udp_client.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_linked_list.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_map.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_client.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_server.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_server_abstract.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_server_interactive.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_worker.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_set.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_plugin.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_cache.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_scheduler.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_channel.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_shared_memory.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue_ipc.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_iterator.c)

list (APPEND SAT_LIBRARIES uuid m pthread dl pthread)

list (APPEND SAT_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include
                          ${CMAKE_CURRENT_LIST_DIR}/include/sat_tcp/
                          ${CMAKE_CURRENT_LIST_DIR}/include/sat_udp/)

file (WRITE ${CMAKE_CURRENT_LIST_DIR}/include/sat_optionals.h "")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoadModules.txt")

add_library (sat SHARED "")
target_link_libraries (sat ${SAT_LIBRARIES})
target_sources (sat
    PUBLIC 
        ${SAT_SOURCES}
    )
target_include_directories (sat
    PUBLIC 
    ${SAT_INCLUDES}
    )

add_subdirectory (tests)

install (TARGETS sat LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install (FILES include/sat.h             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_uuid.h        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_file.h        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_time.h        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_status.h      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_log.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_validation.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_event.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_stack.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_array.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_udp.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_linked_list.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_queue.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_map.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_tcp.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_tcp/sat_tcp_types.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_worker.h      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_set.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_plugin.h      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_cache.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_scheduler.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_channel.h     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_shared_memory.h     DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_queue_ipc.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_iterator.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})