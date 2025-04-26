add_library(internal_base INTERFACE)

target_link_libraries(internal_base INTERFACE internal_dependency)

target_include_directories(internal_base INTERFACE ${PROJECT_SOURCE_DIR}/include/)

target_compile_features(internal_base INTERFACE cxx_std_23)

target_compile_definitions(internal_base
    INTERFACE
        $<$<CONFIG:Debug>:SKY_BIG_DEBUG>
)


configure_file(config.h.in ${PROJECT_SOURCE_DIR}/include/sky_big/config.h @ONLY)
