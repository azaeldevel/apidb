CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(apidb VERSION 2.0.0.9 LANGUAGES CXX)
SET(CMAKE_CXX_STANDARD 11)
SET(CMAKE_CXX_STANDARD_REQUIRED ON)
SET(CMAKE_CXX_EXTENSIONS OFF)
SET(DDM_DESKTOP_DIR "/usr/share/applications/")
SET(DDM_PIXMAPS_DIR "/usr/share/icons/")
SET(${PROJECT_NAME}_PAKAGENAME_DECORED "APIDB")

EXECUTE_PROCESS(COMMAND date +"%Y%m%d%H%M%S" OUTPUT_VARIABLE ${PROJECT_NAME}_VERSION_BUILD)

if(NOT APIDB_VERSION_STAGE)
        MESSAGE(FATAL_ERROR "Es necesario que indique una etapa de desasorrollo, use -DAPIDB_VERSION_STAGE='etapa'.\nEn donde etapa puede ser .. snapshot,alpha,beta,rc,release.\nEn términos generales acepta cualquier miembro de la enumeración toolkit::Version::Stage para mas detalles vea la información de este paquete.")
elseif(${APIDB_VERSION_STAGE} STREQUAL "release")
        if(NOT CMAKE_BUILD_TYPE)
                MESSAGE(FATAL_ERROR "La etapa del proyecto, es 'release', asigne tambien CMAKE_BUILD_TYPE=Release para mejor estabilidad.")
        endif()
endif()
if(NOT CMAKE_BUILD_TYPE)

elseif(${CMAKE_BUILD_TYPE} STREQUAL "Release")
        if(NOT APIDB_VERSION_STAGE)
                MESSAGE(FATAL_ERROR "Asigno CMAKE_BUILD_TYPE = Release para que los componentes de APIDB funcione correctamente es necesario que asigne APIDB_VERSION_STAGE = release")
	elseif(NOT ${APIDB_VERSION_STAGE} STREQUAL "release")
		MESSAGE(FATAL_ERROR "Asigno CMAKE_BUILD_TYPE = Release para que los componentes de APIDB funcione correctamente es necesario que asigne APIDB_VERSION_STAGE = release")
        endif()
endif()


CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/versionInfo.h.in" "${PROJECT_SOURCE_DIR}/versionInfo.h")
CONFIGURE_FILE("${PROJECT_SOURCE_DIR}/apidb.desktop.in" "${CMAKE_CURRENT_BINARY_DIR}/apidb.desktop")


##################################################################################################################
####FIND PAKAGES
FIND_PACKAGE(MySQL REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../cmake/Modules/)
IF(MySQL_FOUND)
	INCLUDE_DIRECTORIES(${MYSQL_INCLUDE_DIR})
ELSEIF()
	MESSAGE(FATAL_ERROR "No se encontro la MySQL client.")
ENDIF()
FIND_PACKAGE(octetos-toolkit-common-c++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../cmake/Modules/toolkit/)
IF(OCTETOS_TOOLKIT_COMMON_CPP_FOUND)
	INCLUDE_DIRECTORIES(${OCTETOS_TOOLKIT_COMMON_CPP_INCLUDE_DIR})
ELSEIF(OCTETOS_TOOLKIT_COMMON_CPP_FOUND)
	MESSAGE(FATAL_ERROR "No se encontro la toolkit-common-c++.")
ENDIF(OCTETOS_TOOLKIT_COMMON_CPP_FOUND)
FIND_PACKAGE(octetos-toolkit-clientdb-myc++ REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../cmake/Modules/toolkit/)
IF(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)
	INCLUDE_DIRECTORIES(${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_INCLUDE_DIR})
ELSEIF(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)
	MESSAGE(FATAL_ERROR "No se encontro la toolkit-cliendb-myc++.")
ENDIF(OCTETOS_TOOLKIT_CLIENTDB_MYCPP_FOUND)
FIND_PACKAGE( LibXml2 REQUIRED )
IF(LIBXML2_FOUND)
	INCLUDE_DIRECTORIES(${LIBXML2_INCLUDE_DIR})
ENDIF()
FIND_PACKAGE( Boost COMPONENTS REQUIRED )
IF(Boost_FOUND)
	INCLUDE_DIRECTORIES(${Boost_INCLUDE_DIR})
ENDIF()
FIND_PACKAGE(tar REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../cmake/Modules/)
IF(TAR_FOUND)
	INCLUDE_DIRECTORIES(${TAR_INCLUDE_DIR})
ELSEIF()
	MESSAGE(FATAL_ERROR "No se encontro la Libreria tar.")
ENDIF()
FIND_PACKAGE(PkgConfig REQUIRED)
PKG_CHECK_MODULES(GTK3 REQUIRED gtk+-3.0)
IF(GTK3_FOUND)
	INCLUDE_DIRECTORIES(${GTK3_INCLUDE_DIRS})
	LINK_DIRECTORIES(${GTK3_LIBRARY_DIRS})
ELSEIF()
	MESSAGE(FATAL_ERROR "No se encontro la Libreria GTK3.")
ENDIF()
FIND_PACKAGE(CUnit REQUIRED PATHS ${PROJECT_SOURCE_DIR}/../cmake/Modules/)
IF(CUNIT_FOUND)
	INCLUDE_DIRECTORIES(${CUNIT_INCLUDE_DIR})
ENDIF()
if(APIDB_DOC)
FIND_PACKAGE(Doxygen)
IF(Doxygen_FOUND)
        SET(doxyfile_in "doxygen-public-c++")
ELSE()
        MESSAGE(FATAL_ERROR "No se encontro Doxygen.")
ENDIF()
endif()



###################################################################################################
###BUILD
INCLUDE_DIRECTORIES(mysql-reader-c++)
INCLUDE_DIRECTORIES(${CMAKE_CURRENT_BINARY_DIR}/mysql-reader-c++/)
SET(LIBREADER "NULL")
SET(LIBPARTIAL1 ${PROJECT_NAME}-partial1)
ADD_SUBDIRECTORY(mysql-reader-c++)

SET(SOURCES_BUILDING common-mysql.cpp  apidb.cpp analyzer.cpp generator.cpp generator-c++.cpp common.cpp  Errors.cpp ConfigureProject.cpp ConfigureProjectRead_v${${PROJECT_NAME}_VERSION_MAJOR}.cpp driver.cpp)
SET(LIBRARIES  ${OCTETOS_TOOLKIT_CLIENTDB_MYCPP_LIBRARIES} ${OCTETOS_TOOLKIT_COMMON_CPP_LIBRARIES} ${MYSQL_LIBRARY} ${LIBXML2_LIBRARIES} ${Boost_LIBRARIES} ${LibXML++_LIBRARIES} ${TAR_LIBRARIES} ${GTK3_LIBRARIES} ${CUNIT_LIBRARY})

ADD_LIBRARY(${PROJECT_NAME}-obj  OBJECT ${SOURCES_BUILDING})
set_target_properties(${PROJECT_NAME}-obj  PROPERTIES POSITION_INDEPENDENT_CODE 1 )

ADD_LIBRARY(${PROJECT_NAME} SHARED $<TARGET_OBJECTS:${PROJECT_NAME}-obj> $<TARGET_OBJECTS:${LIBREADER}-obj>)
TARGET_LINK_LIBRARIES(${PROJECT_NAME}  )

ADD_EXECUTABLE(${PROJECT_NAME}-build apidb-build.cpp ${SOURCES_BUILDING})
ADD_DEPENDENCIES(${PROJECT_NAME}-build ${LIBREADER})
TARGET_LINK_LIBRARIES(apidb-build ${LIBRARIES} ${LIBREADER} )

ADD_EXECUTABLE(testing-v${${PROJECT_NAME}_VERSION_MAJOR} tests/v${${PROJECT_NAME}_VERSION_MAJOR}.cpp ${SOURCES_BUILDING} )
TARGET_LINK_LIBRARIES(testing-v${${PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES} ${LIBREADER} )

ADD_EXECUTABLE(${PROJECT_NAME}-gtk3 apidb-gtk3.cpp Application-gtk3.cpp ${SOURCES_BUILDING})
TARGET_LINK_LIBRARIES(${PROJECT_NAME}-gtk3 ${LIBRARIES} ${LIBREADER} )

execute_process(COMMAND inkscape --file "${CMAKE_CURRENT_SOURCE_DIR}/apidb.svg" --export-png "${CMAKE_CURRENT_BINARY_DIR}/apidb-128x128.png" -h 128 -w 128)
execute_process(COMMAND inkscape --file "${CMAKE_CURRENT_SOURCE_DIR}/apidb.svg" --export-png "${CMAKE_CURRENT_BINARY_DIR}/apidb-256x256.png" -h 256 -w 256)


IF(Doxygen_FOUND AND APIDB_DOC)
add_custom_target(
        doc ALL
        COMMAND ${DOXYGEN_EXECUTABLE} "${PROJECT_SOURCE_DIR}/${doxyfile_in}"
        WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM
        )
ENDIF()


###################################################################################################################
###INSTALL
INSTALL(TARGETS ${PROJECT_NAME}-build DESTINATION /us/bin)
INSTALL(TARGETS ${PROJECT_NAME}-gtk3 DESTINATION /usr/bin)
INSTALL(TARGETS ${PROJECT_NAME}  DESTINATION /lib)
INSTALL(FILES apidb.hpp DESTINATION /usr/include/octetos/apidb)
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/apidb.desktop"  DESTINATION ${DDM_DESKTOP_DIR})
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/apidb-256x256.png"  DESTINATION ${DDM_PIXMAPS_DIR})
INSTALL(FILES "${CMAKE_CURRENT_BINARY_DIR}/apidb-128x128.png"  DESTINATION /usr/shared/octetos/apidb)

include (InstallRequiredSystemLibraries)
SET (CPACK_PACKAGE_VERSION_MAJOR "${${PROJECT_NAME}_VERSION_MAJOR}")
SET (CPACK_PACKAGE_VERSION_MINOR "${${PROJECT_NAME}_VERSION_MINOR}")
SET (CPACK_PACKAGE_VERSION_PATCH "${${PROJECT_NAME}_VERSION_PATCH}")
INCLUDE (CPack)

