get_property (SAT_BUILTIN_LIBRARIES GLOBAL PROPERTY SAT_BUILTIN_LIBS)             
get_property (SAT_OPTIONALS_LIBRARIES GLOBAL PROPERTY SAT_OPTIONAL_LIBS)

add_library (sat SHARED src/sat.c)

target_include_directories (sat
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat 
    PUBLIC
    -Wl,--whole-archive
    ${SAT_BUILTIN_LIBRARIES}
    ${SAT_OPTIONALS_LIBRARIES}
    -Wl,--no-whole-archive
)

install (FILES include/sat.h           DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_builtin.h   DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_optionals.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

install (TARGETS sat LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR})