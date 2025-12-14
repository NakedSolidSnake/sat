add_library (sat_status "")

target_sources (sat_status
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_status.c
)

target_include_directories (sat_status
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)