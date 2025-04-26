configure_file(config.h.in include/sky_big/config.h @ONLY)

add_library(sb_core
    INTERFACE
        include/sky_big/config.h
        include/sky_big/utility.h
)

add_library(sb::core ALIAS sb_core)

target_link_libraries(sb_core INTERFACE internal_dependency)
target_include_directories(sb_core INTERFACE include/)
target_compile_features(sb_core INTERFACE cxx_std_23)
target_compile_definitions(sb_core
    INTERFACE
        $<$<CONFIG:Debug>:SKY_BIG_DEBUG>
)


add_subdirectory(test)
