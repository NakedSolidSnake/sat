cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

enable_testing ()

add_library (sat "")
target_link_libraries (sat uuid)
target_sources (sat
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_uuid.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_file.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_time.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_status.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_log.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_validation.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_event.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_stack.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_array.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_client.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_linked_list.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue.c
    )
target_include_directories (sat
    PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/include
    )

add_subdirectory (tests)