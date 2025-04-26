add_library(sb_leveldb
    STATIC
        include/sky_big/leveldb.h
        include/sky_big/write_batch.h
        coding.h
        leveldb.cpp
        record.cpp
        write_batch.cpp
)

add_library(sb::leveldb ALIAS sb_leveldb)

target_link_libraries(sb_leveldb PUBLIC sb::dependency)

target_compile_features(sb_leveldb PUBLIC cxx_std_23)

target_include_directories(sb_leveldb
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/include
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/include/sky_big
)


if(${PROJECT_NAME}_BUILD_TESTS)
    add_library(sb_leveldb_test INTERFACE)
    target_link_libraries(sb_leveldb_test INTERFACE sb::leveldb)
    target_include_directories(sb_leveldb_test INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

    add_library(sb::test::leveldb ALIAS sb_leveldb_test)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/test)
endif()