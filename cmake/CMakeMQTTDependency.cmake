option (SAT_MQTT "SAT_MQTT support" OFF)

if (${SAT_MQTT})
    message (STATUS "SAT_MQTT support enable")

    # check_library (PAHO_MQTT "paho-mqtt3c")

    list (APPEND SAT_SOURCES ${CMAKE_SOURCE_DIR}/src/sat_mqtt.c)
    file (APPEND ${CMAKE_SOURCE_DIR}/include/sat_optionals.h "#include <sat_mqtt.h>\n")
    list (APPEND SAT_INCLUDES ${CMAKE_SOURCE_DIR}/include/sat_mqtt)
    list (APPEND SAT_LIBRARIES paho-mqtt3c)

    install (FILES include/sat_mqtt.h                           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_mqtt/sat_mqtt_message.h          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_mqtt/sat_mqtt_qos.h              DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
    install (FILES include/sat_mqtt/sat_mqtt_persistence_type.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
endif ()