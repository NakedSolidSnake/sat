add_library (sat_time "")

target_sources (sat_time
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_time.c
)

target_include_directories (sat_time
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

# target_link_libraries (sat_time
#     PUBLIC
#     sat_status
# )

install (FILES include/sat_time.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_time")