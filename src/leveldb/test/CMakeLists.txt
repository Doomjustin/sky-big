include(CTest)
include(Catch)

function(add_leveldb_test name)
    add_executable(test.leveldb.${name} test_${name}.cpp)

    target_link_libraries(test.leveldb.${name} 
        PRIVATE 
            sb::test::leveldb
            Catch2::Catch2WithMain
    )

    catch_discover_tests(test.leveldb.${name})
endfunction()


add_leveldb_test(coding)
add_leveldb_test(write_batch)