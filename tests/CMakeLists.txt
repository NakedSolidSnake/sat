add_executable (test_sat_uuid ${CMAKE_CURRENT_LIST_DIR}/test_sat_uuid.c)
target_link_libraries (test_sat_uuid PUBLIC sat)
add_test (NAME test_sat_uuid COMMAND test_sat_uuid)

add_executable (test_sat_file ${CMAKE_CURRENT_LIST_DIR}/test_sat_file.c)
target_link_libraries (test_sat_file PUBLIC sat)
add_test (NAME test_sat_file COMMAND test_sat_file)

add_executable (test_sat_time ${CMAKE_CURRENT_LIST_DIR}/test_sat_time.c)
target_link_libraries (test_sat_time PUBLIC sat)
add_test (NAME test_sat_time COMMAND test_sat_time)

add_executable (test_sat_status ${CMAKE_CURRENT_LIST_DIR}/test_sat_status.c)
target_link_libraries (test_sat_status PUBLIC sat)
add_test (NAME test_sat_status COMMAND test_sat_status)

add_executable (test_sat_log ${CMAKE_CURRENT_LIST_DIR}/test_sat_log.c)
target_link_libraries (test_sat_log PUBLIC sat)
add_test (NAME test_sat_log COMMAND test_sat_log)

add_executable (test_sat_validation ${CMAKE_CURRENT_LIST_DIR}/test_sat_validation.c)
target_link_libraries (test_sat_validation PUBLIC sat)
add_test (NAME test_sat_validation COMMAND test_sat_validation)

add_executable (test_sat_event ${CMAKE_CURRENT_LIST_DIR}/test_sat_event.c)
target_link_libraries (test_sat_event PUBLIC sat)
add_test (NAME test_sat_event COMMAND test_sat_event)

add_executable (test_sat_stack ${CMAKE_CURRENT_LIST_DIR}/test_sat_stack.c)
target_link_libraries (test_sat_stack PUBLIC sat)
add_test (NAME test_sat_stack COMMAND test_sat_stack)

add_executable (test_sat_array ${CMAKE_CURRENT_LIST_DIR}/test_sat_array.c)
target_link_libraries (test_sat_array PUBLIC sat)
add_test (NAME test_sat_array COMMAND test_sat_array)

add_executable (test_sat_udp ${CMAKE_CURRENT_LIST_DIR}/test_sat_udp.c)
target_link_libraries (test_sat_udp PUBLIC sat)
add_test (NAME test_sat_udp COMMAND test_sat_udp)

add_executable (test_sat_linked_list ${CMAKE_CURRENT_LIST_DIR}/test_sat_linked_list.c)
target_link_libraries (test_sat_linked_list PUBLIC sat)
add_test (NAME test_sat_linked_list COMMAND test_sat_linked_list)

add_executable (test_sat_queue ${CMAKE_CURRENT_LIST_DIR}/test_sat_queue.c)
target_link_libraries (test_sat_queue PUBLIC sat)
add_test (NAME test_sat_queue COMMAND test_sat_queue)

add_executable (test_sat_map ${CMAKE_CURRENT_LIST_DIR}/test_sat_map.c)
target_link_libraries (test_sat_map PUBLIC sat)
add_test (NAME test_sat_map COMMAND test_sat_map)

add_executable (test_sat_tcp ${CMAKE_CURRENT_LIST_DIR}/test_sat_tcp.c)
target_link_libraries (test_sat_tcp PUBLIC sat)
add_test (NAME test_sat_tcp COMMAND test_sat_tcp)

add_executable (test_sat_worker ${CMAKE_CURRENT_LIST_DIR}/test_sat_worker.c)
target_link_libraries (test_sat_worker PUBLIC sat)
add_test (NAME test_sat_worker COMMAND test_sat_worker)

add_executable (test_sat_set ${CMAKE_CURRENT_LIST_DIR}/test_sat_set.c)
target_link_libraries (test_sat_set PUBLIC sat)
add_test (NAME test_sat_set COMMAND test_sat_set)

add_executable (test_sat_plugin ${CMAKE_CURRENT_LIST_DIR}/test_sat_plugin.c)
target_link_libraries (test_sat_plugin PUBLIC sat)
add_test (NAME test_sat_plugin COMMAND test_sat_plugin)

add_executable (test_sat_cache ${CMAKE_CURRENT_LIST_DIR}/test_sat_cache.c)
target_link_libraries (test_sat_cache PUBLIC sat)
add_test (NAME test_sat_cache COMMAND test_sat_cache)

add_executable (test_sat_scheduler ${CMAKE_CURRENT_LIST_DIR}/test_sat_scheduler.c)
target_link_libraries (test_sat_scheduler PUBLIC sat)
add_test (NAME test_sat_scheduler COMMAND test_sat_scheduler)

add_executable (test_sat_channel ${CMAKE_CURRENT_LIST_DIR}/test_sat_channel.c)
target_link_libraries (test_sat_channel PUBLIC sat)
add_test (NAME test_sat_channel COMMAND test_sat_channel)

add_executable (test_sat_shared_memory ${CMAKE_CURRENT_LIST_DIR}/test_sat_shared_memory.c)
target_link_libraries (test_sat_shared_memory PUBLIC sat)
add_test (NAME test_sat_shared_memory COMMAND test_sat_shared_memory)

add_executable (test_sat_queue_ipc ${CMAKE_CURRENT_LIST_DIR}/test_sat_queue_ipc.c)
target_link_libraries (test_sat_queue_ipc PUBLIC sat)
add_test (NAME test_sat_queue_ipc COMMAND test_sat_queue_ipc)

add_executable (test_sat_iterator ${CMAKE_CURRENT_LIST_DIR}/test_sat_iterator.c)
target_link_libraries (test_sat_iterator PUBLIC sat)
add_test (NAME test_sat_iterator COMMAND test_sat_iterator)

add_subdirectory (test_mysql)
add_subdirectory (test_postgres)
add_subdirectory (test_sqlite)
add_subdirectory (test_kafka)
add_subdirectory (test_redis)
add_subdirectory (test_webserver)
add_subdirectory (test_json)
add_subdirectory (test_curl)
add_subdirectory (test_mqtt)
add_subdirectory (test_ini)
add_subdirectory (test_sdl)
add_subdirectory (test_gtk3)
add_subdirectory (test_opengl)