SET(TARGET_OBJECT_SUFFIX toolkit-common-c++)

IF (TOOLKIT_COMMON_INCLUDE_DIR)
  # Already in cache, be silent
  SET(TOOLKIT_COMMON_FIND_QUIETLY TRUE)
ENDIF (TOOLKIT_COMMON_INCLUDE_DIR)

FIND_PATH(TOOLKIT_COMMON_INCLUDE_DIR common.hpp
  /usr/include/toolkit
  /usr/local/include/toolkit
  
)

SET(TOOLKIT_COMMON_NAMES ${TARGET_OBJECT_SUFFIX})
FIND_LIBRARY(TOOLKIT_COMMON_LIBRARY
  NAMES ${TOOLKIT_COMMON_NAMES}
  PATHS /usr/lib/toolkit /usr/local/lib/toolkit $ENV{HOME}/development/toolkit/common/build-c++
  PATH_SUFFIXES ${TARGET_OBJECT_SUFFIX}
)

IF (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)
  SET(TOOLKIT_COMMON_FOUND TRUE)
  SET( TOOLKIT_COMMON_LIBRARIES ${TOOLKIT_COMMON_LIBRARY} )
ELSE (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)
  SET(TOOLKIT_COMMON_FOUND FALSE)
  SET( TOOLKIT_COMMON_LIBRARIES )
ENDIF (TOOLKIT_COMMON_INCLUDE_DIR AND TOOLKIT_COMMON_LIBRARY)

IF (TOOLKIT_COMMON_FOUND)
  IF (NOT TOOLKIT_COMMON_FIND_QUIETLY)
    MESSAGE(STATUS "Found ${TARGET_OBJECT_SUFFIX}: ${TOOLKIT_COMMON_LIBRARY}")
  ENDIF (NOT TOOLKIT_COMMON_FIND_QUIETLY)
ELSE (TOOLKIT_COMMON_FOUND)
  IF (TOOLKIT_COMMON_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for ${TARGET_OBJECT_SUFFIX} libraries named ${TOOLKIT_COMMON_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find ${TARGET_OBJECT_SUFFIX} library")
  ENDIF (TOOLKIT_COMMON_FIND_REQUIRED)
ENDIF (TOOLKIT_COMMON_FOUND)

MARK_AS_ADVANCED(
  TOOLKIT_COMMON_LIBRARY
  TOOLKIT_COMMON_INCLUDE_DIR
  )