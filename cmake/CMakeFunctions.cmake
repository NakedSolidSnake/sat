# Create samples function

function (create_sample binary folder)
    add_executable (${binary} ${CMAKE_CURRENT_LIST_DIR}/${binary}.c)
    target_link_libraries (${binary} PUBLIC sat)
    set_target_properties (${binary} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/samples/${folder})
endfunction ()


function (create_test binary)
    add_executable (${binary} ${CMAKE_CURRENT_LIST_DIR}/${binary}.c)
    target_link_libraries (${binary} PUBLIC sat)
    add_test (NAME ${binary} COMMAND ${binary})
    set_target_properties (${binary} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests/)
endfunction ()