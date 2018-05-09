set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wno-deprecated-register")

project(compiler-sql CXX)

cmake_minimum_required(VERSION 3.1)

find_package(BISON REQUIRED)
find_package(FLEX REQUIRED)

BISON_TARGET(parser parser.yy ${PROJECT_SOURCE_DIR}/parser.tab.cc)
FLEX_TARGET(lexer lexer.l ${PROJECT_SOURCE_DIR}/lexer.yy.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

FIND_PACKAGE(MySQL REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../../cmake/Modules/)
IF(MySQL_FOUND)
	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
ENDIF()
FIND_PACKAGE(toolkit-common REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)
IF(TOOLKIT_COMMON_FOUND)
	INCLUDE_DIRECTORIES(${TOOLKIT_COMMON_INCLUDE_DIR})
ENDIF()
FIND_PACKAGE(toolkit-clientdb REQUIRED PATHS ${PROJECT_SOURCE_DIR}/cmake.modules/)
IF(TOOLKIT_CLIENTDB_FOUND)
	INCLUDE_DIRECTORIES(${TOOLKIT_CLIENTDB_INCLUDE_DIR})
ENDIF()

add_executable(developing main.cpp driver.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
target_include_directories(developing PRIVATE . ${CMAKE_CURRENT_BINARY_DIR})
set_property(TARGET developing PROPERTY CXX_STANDARD 14)

ADD_LIBRARY(apidb-generator-sql STATIC driver.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
TARGET_LINK_LIBRARIES(apidb-generator-sql ${TOOLKIT_COMMON_LIBRARY} ${TOOLKIT_CLIENTDB_LIBRARY} ${MYSQL_LIBRARY})


enable_testing()
add_test(NAME basic COMMAND ${CMAKE_SOURCE_DIR}/test/test0.pl WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR})
