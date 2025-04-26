include(AddCatch2)

list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
include(CTest)
include(Catch)

function(add_core_test name)
    add_executable(test.core.${name} test_${name}.cpp)

    target_link_libraries(test.core.${name} 
        PRIVATE 
            sb::core
            Catch2::Catch2WithMain
    )

    catch_discover_tests(test.core.${name})
endfunction()


add_executable(test.core.thread_pool test_thread_pool.cpp)
target_link_libraries(test.core.thread_pool PRIVATE sb::core)

add_core_test(utility)
add_core_test(lru_cache)
add_core_test(generator)