cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

option (SAT_MYSQL "SAT_MYSQL support" OFF)
option (SAT_POSTGRES "SAT_POSTGRES support" OFF)
option (SAT_SQLITE "SAT_SQLITE support" OFF)


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
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_server.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_udp_client.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_linked_list.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_queue.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_map.c)

list (APPEND SAT_LIBRARIES uuid)

list (APPEND SAT_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)

file (WRITE ${CMAKE_CURRENT_LIST_DIR}/include/sat_optionals.h "")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include (CMakeMySQLDependency)
include (CMakePostgresDependency)
include (CMakeSQLiteDependency)

add_library (sat "")
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