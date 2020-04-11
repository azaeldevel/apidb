project(mariadb-reader-c++ CXX)
EXECUTE_PROCESS(COMMAND date +"%Y%m%d%H%M%S" OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_BUILD)
SET(${PROJECT_NAME}_VERSION_STAGE ${apidb_VERSION_STAGE})
SET(${PROJECT_NAME}_VERSION_NAME ${apidb_VERSION_NAME})




#################################################################################################################


####################################################################################################
IF(APIDB_MARIADB STREQUAL "Y")
    PKG_CHECK_MODULES(LIBMARIADB REQUIRED libmariadb)
    IF(LIBMARIADB)
        INCLUDE_DIRECTORIES(${LIBMARIADB_INCLUDE_DIR})
    ELSEIF()
        MESSAGE(FATAL_ERROR "No se encontro la libmariadb")
    ENDIF()
    PKG_CHECK_MODULES(OCTETOS_DB_MARIA REQUIRED octetos-db-maria)
    IF(OCTETOS_DB_MARIA_FOUND)
        INCLUDE_DIRECTORIES(${OCTETOS_DB_MARIA_INCLUDE_DIR})
    ELSEIF(OCTETOS_DB_MARIA_FOUND)
        MESSAGE(FATAL_ERROR "No se encontro la octetos-db-mariadb.")
    ENDIF()
    add_definitions(-DAPIDB_MARIADB)
ENDIF()

find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)
FLEX_TARGET(lexer lexer.l ${CMAKE_CURRENT_BINARY_DIR}/lexer.yy.cc)
BISON_TARGET(parser parser.yy ${CMAKE_CURRENT_BINARY_DIR}/parser.tab.cc)
ADD_FLEX_BISON_DEPENDENCY(lexer parser)

include_directories(${CMAKE_CURRENT_BINARY_DIR})

ADD_LIBRARY(${PROJECT_NAME}-obj  OBJECT analyzer.cpp scanner.cpp analyzer-mariadb.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
set_target_properties(${PROJECT_NAME}-obj  PROPERTIES POSITION_INDEPENDENT_CODE 1 )

ADD_LIBRARY(${PROJECT_NAME}  STATIC analyzer.cpp scanner.cpp  analyzer-mariadb.cpp ${FLEX_lexer_OUTPUTS} ${BISON_parser_OUTPUTS})
TARGET_INCLUDE_DIRECTORIES(${PROJECT_NAME} PRIVATE  . ${CMAKE_CURRENT_BINARY_DIR})
SET_PROPERTY(TARGET ${PROJECT_NAME}  PROPERTY CXX_STANDARD 11)
SET(LIBREADER ${PROJECT_NAME} PARENT_SCOPE)

