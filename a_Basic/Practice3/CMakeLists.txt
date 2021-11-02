cmake_minimum_required(VERSION 3.17.3)

project(
    Testnamespace
    VERSION 0.1
    DESCRIPTION "namespace 테스트"
    LANGUAGES CXX
)

add_executable(program header_test.cpp)
target_compile_options(program PUBLIC -Wall -Werror)
target_include_directories(program PUBLIC ${CMAKE_SOURCE_DIR}/includes)