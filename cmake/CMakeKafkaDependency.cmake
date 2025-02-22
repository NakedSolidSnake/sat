option (SAT_KAFKA "SAT_KAFKA support" OFF)

if (${SAT_KAFKA})

    message (STATUS "SAT_KAFKA support enable")

    
    check_library (GLIB "glib-2.0")
    # if (GLIB_FOUND)
    #     message (STATUS "GLIB found")
    # else ()
    #     message (FATAL_ERROR "GLIB not found")
    # endif ()
    
    check_library (RDKAFKA "rdkafka")

    # if (RDKAFKA_FOUND)
    #     message (STATUS "RDKAFKA found")
    # else ()
    #     message (FATAL_ERROR "RDKAFKA not found")
    # endif ()
        
    list (APPEND SAT_SOURCES 
        ${CMAKE_SOURCE_DIR}/src/sat_kafka.c
        ${CMAKE_SOURCE_DIR}/src/sat_kafka/sat_kafka_common.c
        ${CMAKE_SOURCE_DIR}/src/sat_kafka/sat_kafka_configuration.c)
    
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_kafka.h>\n")

    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_kafka)

    list (APPEND SAT_LIBRARIES ${RDKAFKA_LIBRARIES} ${GLIB_LIBRARIES})

    add_definitions (${RDKAFKA_CFLAGS} ${RDKAFKA_CFLAGS_OTHER} ${GLIB_CFLAGS} ${GLIB_CFLAGS_OTHER})

    install (FILES include/sat_kafka.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_kafka/sat_kafka_base.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_kafka/sat_kafka_groups.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_kafka/sat_kafka_topics.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_kafka/sat_kafka_message.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
            
endif ()

    