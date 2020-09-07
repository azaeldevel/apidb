project(mysql-reader-c++ CXX)
EXECUTE_PROCESS(COMMAND date +"%Y%m%d%H%M%S" OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_BUILD)
SET(${PROJECT_NAME}_VERSION_STAGE ${apidb_VERSION_STAGE})
SET(${PROJECT_NAME}_VERSION_NAME ${apidb_VERSION_NAME})




#################################################################################################################


####################################################################################################


find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
FLEX_TARGET(lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
BISON_TARGET(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

ADD_LIBRARY(${PROJECT_NAME}-obj  OBJECT analyzer.cpp scanner.cpp analyzer-mysql.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
set_target_properties(${PROJECT_NAME}-obj  PROPERTIES POSITION_INDEPENDENT_CODE 1 )

#ADD_LIBRARY(${PROJECT_NAME}  STATIC analyzer.cpp scanner.cpp analyzer-mysql.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
#TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE  . ${CMAKE_CURRENT_BINARY_DIR})
#SET_PROPERTY(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 11)
SET(LIBREADER_MYSQL ${PROJECT_NAME}-obj PARENT_SCOPE)
