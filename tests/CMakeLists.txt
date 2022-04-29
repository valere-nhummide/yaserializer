set(TEST_NAME ${PROJECT_NAME}-test)

find_package(Catch2 REQUIRED)

add_executable(${TEST_NAME}
  ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/integrals-test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ranges/fixed-size-containers-test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ranges/movable-containers-test.cpp
  ${CMAKE_CURRENT_SOURCE_DIR}/ranges/resizable-containers-test.cpp
)

target_include_directories(${TEST_NAME}
  PRIVATE
    ${PROJECT_SOURCE_DIR}
)

target_link_libraries(${TEST_NAME}
  ${PROJECT_NAME}
  Catch2::Catch2
)

if (MSVC)
    target_compile_options(${TEST_NAME}
        INTERFACE
            "/W4"
            "/WX"
            "/std:c++latest"
  )
else (MSVC)
    target_compile_options(${TEST_NAME}
        INTERFACE
            -Wall
            -Wextra
            -pedantic
            -Werror
    )
endif (MSVC)

include(GNUInstallDirs)

install(
  TARGETS
    ${TEST_NAME}
  RUNTIME DESTINATION
    ${CMAKE_INSTALL_BINDIR}/tests
)

add_test(NAME ${TEST_NAME} COMMAND ${TEST_NAME})