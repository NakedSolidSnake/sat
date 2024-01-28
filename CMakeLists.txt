cmake_minimum_required (VERSION 3.16)
project (SAT)
set (CMAKE_C_FLAGS "-D_GNU_SOURCE -Wall -std=c99")
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set (LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
set (CMAKE_BUILD_TYPE Debug)

include (GNUInstallDirs)

option (SAT_MYSQL "SAT_MYSQL support" OFF)
option (SAT_POSTGRES "SAT_POSTGRES support" OFF)
option (SAT_SQLITE "SAT_SQLITE support" OFF)
option (SAT_KAFKA "SAT_KAFKA support" OFF)
option (SAT_REDIS "SAT_REDIS support" OFF)
option (SAT_WEBSERVER "SAT_WEBSERVER support" OFF)
option (SAT_JSON "SAT_JSON support" OFF)
option (SAT_CURL "SAT_CURL support" OFF)
option (SAT_MQTT "SAT_MQTT support" OFF)

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
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_map.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_client.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_tcp/sat_tcp_server.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_worker.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_set.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_plugin.c
                         ${CMAKE_CURRENT_LIST_DIR}/src/sat_cache.c)

list (APPEND SAT_LIBRARIES uuid m pthread dl)

list (APPEND SAT_INCLUDES ${CMAKE_CURRENT_LIST_DIR}/include ${CMAKE_CURRENT_LIST_DIR}/include/sat_tcp/)

file (WRITE ${CMAKE_CURRENT_LIST_DIR}/include/sat_optionals.h "")

list (APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

include (CMakeMySQLDependency)
include (CMakePostgresDependency)
include (CMakeSQLiteDependency)
include (CMakeKafkaDependency)
include (CMakeRedisDependency)
include (CMakeCivetwebDependency)
include (CMakeJSONDependency)
include (CMakeCURLDependency)
include (CMakeMQTTDependency)

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
install (FILES include/sat_worker.h      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_set.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_plugin.h      DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_cache.h       DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})