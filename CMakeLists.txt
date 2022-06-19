cmake_minimum_required(VERSION 3.15...3.22)

project(
    yaserializer
    VERSION 0.0.1
    DESCRIPTION "Yet another serializer"
    LANGUAGES CXX
)

string(TOUPPER ${PROJECT_NAME}_BUILD_TESTS BUILD_TESTS_OPTION)
option(BUILD_TESTS_OPTION "Build and run unit-tests before installation" ON)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(${PROJECT_SOURCE_DIR}/cmake/CPM.cmake)

add_library(${PROJECT_NAME} INTERFACE)

target_include_directories(${PROJECT_NAME}
    INTERFACE
        ${PROJECT_SOURCE_DIR}/include
)

if(${BUILD_TESTS_OPTION})
    CPMAddPackage(
        NAME catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 2.13.8
    )
    include(CTest)
    add_subdirectory(tests)
endif(${BUILD_TESTS_OPTION})
