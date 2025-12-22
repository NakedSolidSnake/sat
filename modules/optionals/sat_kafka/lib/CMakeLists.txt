add_library (sat_kafka "")

target_sources (sat_kafka
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_kafka.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_kafka_common.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_kafka_configuration.c
)

target_include_directories (sat_kafka
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_kafka
    PUBLIC
    sat_status
    ${RDKAFKA_LIBRARIES}
    ${GLIB_LIBRARIES}
)

add_definitions (${RDKAFKA_CFLAGS} ${RDKAFKA_CFLAGS_OTHER} ${GLIB_CFLAGS} ${GLIB_CFLAGS_OTHER})

install (FILES include/sat_kafka.h         DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_kafka_base.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_kafka_groups.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_kafka_topics.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_kafka_message.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_kafka.h>\n")

set_property (GLOBAL PROPERTY SAT_KAFKA_COMPILED TRUE)