add_library (sat_mqtt "")

target_sources (sat_mqtt
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_mqtt.c
)

target_include_directories (sat_mqtt
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_mqtt
    PUBLIC
    sat_status
    paho-mqtt3c
)

install (FILES include/sat_mqtt.h                  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_mqtt_message.h          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_mqtt_qos.h              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_mqtt_persistence_type.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_mqtt.h>\n")
set_property (GLOBAL PROPERTY SAT_MQTT_COMPILED TRUE)