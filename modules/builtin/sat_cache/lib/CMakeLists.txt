add_library (sat_cache "")

target_sources (sat_cache
    PRIVATE
    src/sat_cache.c
)

target_include_directories (sat_cache
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)

target_link_libraries (sat_cache
    PUBLIC
    sat_status
)

install (FILES include/sat_cache.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

set_property (GLOBAL APPEND PROPERTY SAT_BUILTIN_LIBS "sat_cache")