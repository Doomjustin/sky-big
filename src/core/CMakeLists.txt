configure_file(config.h.in ${CMAKE_CURRENT_LIST_DIR}/include/sky_big/config.h @ONLY)

add_library(sb_core
    STATIC
        include/sky_big/config.h
        include/sky_big/queue.h
        include/sky_big/thread_pool.h
        include/sky_big/utility.h
        thread_pool.cpp
)

add_library(sb::core ALIAS sb_core)

target_link_libraries(sb_core PUBLIC internal_dependency)

target_include_directories(sb_core 
    INTERFACE 
        include/
    PRIVATE
        include/sky_big
)

target_compile_features(sb_core PUBLIC cxx_std_23)

target_compile_definitions(sb_core
    PUBLIC
        $<$<CONFIG:Debug>:SKY_BIG_DEBUG>
)


if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()