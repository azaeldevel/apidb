
#  TAR_INCLUDE_DIR - where to find clientdb-mysql.hpp, etc.
#  TAR_LIBRARIES   - List of libraries when using TK Common.
#  TAR_FOUND       - True if TK Common found.

IF (TAR_INCLUDE_DIR)
  # Already in cache, be silent
  SET(TAR_FIND_QUIETLY TRUE)
ENDIF (TAR_INCLUDE_DIR)

FIND_PATH(TAR_INCLUDE_DIR libtar.h
  /usr/local/include
  /usr/include
)

SET(TAR_NAMES tar)
FIND_LIBRARY(TAR_LIBRARY
  NAMES ${TAR_NAMES}
  PATHS /usr/lib /usr/local/lib /usr/lib/x86_64-linux-gnu
  PATH_SUFFIXES tar
)

IF (TAR_INCLUDE_DIR AND TAR_LIBRARY)
  SET(TAR_FOUND TRUE)
  SET(TAR_LIBRARIES ${TAR_LIBRARY} )
ELSE (TAR_INCLUDE_DIR AND TAR_LIBRARY)
  SET(TAR_FOUND FALSE)
  SET(TAR_LIBRARIES )
ENDIF (TAR_INCLUDE_DIR AND TAR_LIBRARY)

IF (TAR_FOUND)
  IF (NOT TAR_FIND_QUIETLY)
    MESSAGE(STATUS "Found Toolkit ClientDB C++: ${TAR_LIBRARY}")
  ENDIF (NOT TAR_FIND_QUIETLY)
ELSE (TAR_FOUND)
  IF (TAR_FIND_REQUIRED)
    MESSAGE(STATUS "Looked for Toolkit ClientDB C++ libraries named ${TAR_NAMES}.")
    MESSAGE(FATAL_ERROR "Could NOT find Toolkit ClientDB C++ library")
  ENDIF (TAR_FIND_REQUIRED)
ENDIF (TAR_FOUND)

MARK_AS_ADVANCED(
  TAR_LIBRARY
  TAR_INCLUDE_DIR
  )
