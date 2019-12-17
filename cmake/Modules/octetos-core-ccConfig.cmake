
#  OCTETOS_CORE_CC_INCLUDE_DIR - where to find file.
#  OCTETOS_CORE_CC_LIBRARIES   - List of libraries when using TK Common.
#  OCTETOS_CORE_CC_FOUND       - True if TK Common found.

#SET(CMAKE_FIND_LIBRARY_SUFFIXES .a .so)

IF (OCTETOS_CORE_CC_INCLUDE_DIR)
  # Already in cache, be silent
  SET(OCTETOS_CORE_CC_FIND_QUIETLY TRUE)
ENDIF (OCTETOS_CORE_CC_INCLUDE_DIR)


if("${APIDB_VERSION_STAGE}" STREQUAL "developing" )
    FIND_PATH(OCTETOS_CORE_CC_INCLUDE_DIR common.hh
        $ENV{HOME}/develop/octetos-core/Debug/src
    )
    SET(OCTETOS_CORE_CC_NAMES octetos-core-cc)
    FIND_LIBRARY(OCTETOS_CORE_CC_LIBRARY
        NAMES ${OCTETOS_CORE_CC_NAMES}
        PATHS $ENV{HOME}/develop/octetos-core/Debug/src
        PATH_SUFFIXES octetos-core-cc
    )
else()
    FIND_PATH(OCTETOS_CORE_CC_INCLUDE_DIR core/common.hh
        /usr/local/include/octetos
        /usr/include/octetos
    )  
    
    SET(OCTETOS_CORE_PARSER_C_NAMES octetos-parser-version-c)
    FIND_LIBRARY(OCTETOS_CORE_PARSER_C_LIBRARY
        NAMES ${OCTETOS_CORE_PARSER_C_NAMES}
        PATHS /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core
        PATH_SUFFIXES octetos-parser-version-c
    )   
    SET(OCTETOS_CORE_C_NAMES octetos-core-c)
    FIND_LIBRARY(OCTETOS_CORE_C_LIBRARY
        NAMES ${OCTETOS_CORE_C_NAMES}
        PATHS /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core
        PATH_SUFFIXES octetos-core-c
    )
    SET(OCTETOS_CORE_CC_NAMES octetos-core-cc)
    FIND_LIBRARY(OCTETOS_CORE_CC_LIBRARY
        NAMES ${OCTETOS_CORE_CC_NAMES}
        PATHS /lib /lib64 /lib32 /usr/lib /usr/local/lib/ /usr/lib/octetos/core /usr/local/lib/octetos/core /usr/lib/x86_64-linux-gnu/octetos/core
        PATH_SUFFIXES octetos-core-cc )   
endif()

IF (OCTETOS_CORE_CC_INCLUDE_DIR)
  SET(OCTETOS_CORE_CC_FOUND TRUE)
  SET(OCTETOS_CORE_CC_LIBRARIES ${OCTETOS_CORE_PARSER_C_LIBRARY} ${OCTETOS_CORE_C_LIBRARY} ${OCTETOS_CORE_CC_LIBRARY} )
ELSE (OCTETOS_CORE_CC_INCLUDE_DIR)
  SET(OCTETOS_CORE_CC_FOUND FALSE)
  SET(OCTETOS_CORE_CC_LIBRARIES )
ENDIF (OCTETOS_CORE_CC_INCLUDE_DIR)

IF (OCTETOS_CORE_CC_FOUND)
  IF (NOT OCTETOS_CORE_CC_FIND_QUIETLY)
    MESSAGE(STATUS "Found Octetos Core C++: ${OCTETOS_CORE_PARSER_C_LIBRARY}, ${OCTETOS_CORE_C_LIBRARY}, ${OCTETOS_CORE_CC_LIBRARY}")
  ENDIF (NOT OCTETOS_CORE_CC_FIND_QUIETLY)
ELSE (OCTETOS_CORE_CC_FOUND)
  IF (OCTETOS_CORE_CC_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Octetos Core C++ libraries named ${OCTETOS_CORE_CC_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find Octetos Core C++ library")
  ENDIF (OCTETOS_CORE_CC_FIND_REQUIRED)
ENDIF (OCTETOS_CORE_CC_FOUND)

MARK_AS_ADVANCED(
  OCTETOS_CORE_CC_LIBRARY
  OCTETOS_CORE_CC_INCLUDE_DIR
  )
#SET(CMAKE_FIND_LIBRARY_SUFFIXES .so)
