add_library(sb_log
    STATIC
        include/sky_big/appender.h
        include/sky_big/console_appender.h
        include/sky_big/entry.h
        include/sky_big/formattable_appender.h
        include/sky_big/formatter.h
        include/sky_big/simple_formatter.h
        include/sky_big/simple_logger.h
        console_appender.cpp
        formattable_appender.cpp
        simple_formatter.cpp
        simple_logger.cpp
)

add_library(sb::log ALIAS sb_log)

target_include_directories(sb_log
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sky_big
)

target_link_libraries(sb_log PUBLIC sb::core)


if(${PROJECT_NAME}_BUILD_TESTS)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()