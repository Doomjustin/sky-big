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


add_core_test(utility)