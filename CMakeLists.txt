cmake_minimum_required(VERSION 3.29)

project(SkyBig
    VERSION 0.0.1
    LANGUAGES C CXX
)

include(CTest)
include(CMakeDependentOption)

cmake_dependent_option(${PROJECT_NAME}_BUILD_TESTS
    "Enable ${PROJECT_NAME} project tests targets" ON
    "BUILD_TESTING" OFF
)

set(CMAKE_CXX_VISIBILITY_PRESET hidden)
set(CMAKE_VISIBILITY_INLINES_HIDDEN 1)

list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_LIST_DIR}/cmake)
include(ConfigSafeGuards)
include(Colors)
include(Warnings)
include(LTO)
include(CopyDllsForDebug)

find_lto(CXX)

if (${PROJECT_NAME}_BUILD_TESTS)
    include(AddCatch2)
    list(APPEND CMAKE_MODULE_PATH ${catch2_SOURCE_DIR}/extras)
endif()

add_subdirectory(third-party EXCLUDE_FROM_ALL)

add_subdirectory(app)
add_subdirectory(src)
