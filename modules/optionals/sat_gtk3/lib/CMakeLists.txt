add_library (sat_gtk3 "")

target_sources (sat_gtk3
    PRIVATE
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_gtk3.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_gtk3_window.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_gtk3_button.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_gtk3_label.c
    ${CMAKE_CURRENT_LIST_DIR}/src/sat_gtk3_box.c
)

target_include_directories (sat_gtk3
    PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/include
)

target_link_libraries (sat_gtk3
    PUBLIC
    ${GTK_LIBRARIES}
    sat_status
)

install (FILES include/sat_gtk3.h        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_gtk3_box.h    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_gtk3_button.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_gtk3_label.h  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_gtk3_widget.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install (FILES include/sat_gtk3_window.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

add_definitions (${GTK_CFLAGS} ${GTK_CFLAGS_OTHER})

file (APPEND ${CMAKE_SOURCE_DIR}/modules/main/include/sat_optionals.h "#include <sat_gtk3.h>\n")

set_property (GLOBAL APPEND PROPERTY SAT_OPTIONAL_LIBS "sat_gtk3")