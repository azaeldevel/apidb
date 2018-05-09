set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

project(compiler-sql CXX)

cmake_minimum_required(VERSION 3.1)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
FLEX_TARGET(lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

add_executable(developing main.cpp driver.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
target_include_directories(developing PRIVATE . ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET developing PROPERTY CXX_STANDARD 14)

enable_testing()
add_test(NAME basic COMMAND ${CMAKE_SOURCE_DIR}/test/test0.pl WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
