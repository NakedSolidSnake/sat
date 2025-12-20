add_library (sat_uuid "")

target_sources (sat_uuid
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_uuid.c
)

target_include_directories (sat_uuid
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_uuid
    PUBLIC
    uuid
)

install (FILES include/sat_uuid.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
