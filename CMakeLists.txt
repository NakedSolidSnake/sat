cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -Werror -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

include (GNUInstallDirs)

enable_testing ()

include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeLoadPKG.txt")
include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/CMakeFunctions.txt")
include ("${CMAKE_CURRENT_SOURCE_DIR}/cmake/LoadModules.txt")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

# Check if it is necessary to link some libraries
list (APPEND SAT_LIBRARIES 
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

file (WRITE ${CMAKE_CURRENT_LIST_DIR}/include/sat_optionals.h "")

add_subdirectory (modules)

get_property(SAT_PROPERTIES_BUILT GLOBAL PROPERTY SAT_PROPERTIES_COMPILED)
if(SAT_PROPERTIES_BUILT)
    list(APPEND SAT_LIBRARIES sat_properties)
endif()

list (APPEND SAT_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include)

add_library (sat SHARED "")
target_link_libraries (sat ${SAT_LIBRARIES})
target_sources (sat
    PRIVATE 
        ${CMAKE_CURRENT_LIST_DIR}/src/sat.c
    )
target_include_directories (sat
    PUBLIC 
    ${SAT_INCLUDES}
    )

add_subdirectory (tests)
add_subdirectory (samples)

install (TARGETS sat LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install (FILES include/sat.h             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})