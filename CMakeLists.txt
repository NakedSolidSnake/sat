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

get_property(SAT_CURL_BUILT GLOBAL PROPERTY SAT_CURL_COMPILED)
if(SAT_CURL_BUILT)
    list(APPEND SAT_LIBRARIES sat_curl)
endif()

get_property(SAT_JSON_BUILT GLOBAL PROPERTY SAT_JSON_COMPILED)
if(SAT_JSON_BUILT)
    list(APPEND SAT_LIBRARIES sat_json)
endif()

get_property(SAT_WEBSERVER_BUILT GLOBAL PROPERTY SAT_WEBSERVER_COMPILED)
if(SAT_WEBSERVER_BUILT)
    list(APPEND SAT_LIBRARIES sat_webserver)
endif()

get_property(SAT_LOKI_BUILT GLOBAL PROPERTY SAT_LOKI_COMPILED)
if(SAT_LOKI_BUILT)
    list(APPEND SAT_LIBRARIES sat_loki)
endif()

get_property(SAT_SQLITE_BUILT GLOBAL PROPERTY SAT_SQLITE_COMPILED)
if(SAT_SQLITE_BUILT)
    list(APPEND SAT_LIBRARIES sat_sqlite)
endif()

get_property(SAT_MYSQL_BUILT GLOBAL PROPERTY SAT_MYSQL_COMPILED)
if(SAT_MYSQL_BUILT)
    list(APPEND SAT_LIBRARIES sat_mysql)
endif()

get_property(SAT_REDIS_BUILT GLOBAL PROPERTY SAT_REDIS_COMPILED)
if(SAT_REDIS_BUILT)
    list(APPEND SAT_LIBRARIES sat_redis)
endif()

get_property(SAT_POSTGRES_BUILT GLOBAL PROPERTY SAT_POSTGRES_COMPILED)
if(SAT_POSTGRES_BUILT)
    list(APPEND SAT_LIBRARIES sat_postgres)
endif()

get_property(SAT_KAFKA_BUILT GLOBAL PROPERTY SAT_KAFKA_COMPILED)
if(SAT_KAFKA_BUILT)
    list(APPEND SAT_LIBRARIES sat_kafka)
endif()

get_property(SAT_MQTT_BUILT GLOBAL PROPERTY SAT_MQTT_COMPILED)
if(SAT_MQTT_BUILT)
    list(APPEND SAT_LIBRARIES sat_mqtt)
endif()

get_property (SAT_SDL_BUILT GLOBAL PROPERTY SAT_SDL_COMPILED)
if (SAT_SDL_BUILT)
    list (APPEND SAT_LIBRARIES sat_sdl)
endif ()

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

install (TARGETS sat LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})

install (FILES include/sat.h             DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})