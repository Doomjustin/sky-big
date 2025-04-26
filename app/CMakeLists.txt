function(sb_add_executable target file)
    add_executable(${target} ${file})

    target_link_libraries(${target} PRIVATE internal_dependency)

    target_set_warnings(${target} ENABLE ALL AS_ERROR ALL DISABLE Annoying)
    target_enable_lto(${target} optimized)
    copy_dlls_for_debug(${target} "" "")
endfunction()


sb_add_executable(demo demo.cpp)