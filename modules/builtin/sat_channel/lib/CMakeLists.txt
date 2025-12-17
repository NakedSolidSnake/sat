add_library (sat_channel "")

target_sources (sat_channel
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_channel.c
)

target_include_directories (sat_channel
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_channel
    PUBLIC
    sat_status
)

install (FILES include/sat_channel.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
