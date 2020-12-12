CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(tests LANGUAGES CXX)

#ADD_EXECUTABLE(sandbox-generator sandbox-generator.cpp)
#TARGET_LINK_LIBRARIES(sandbox-generator ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES}) 


#include_directories(${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst)
#ADD_EXECUTABLE(sandbox-queries sandbox-queries.cpp ${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst/muposys.cpp)
#TARGET_LINK_LIBRARIES(sandbox-queries ${LIBRARIES}) 



#include_directories(${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst)
#ADD_EXECUTABLE(sandbox-inserts sandbox-inserts.cpp ${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst/muposys.cpp)
#TARGET_LINK_LIBRARIES(sandbox-inserts ${LIBRARIES}) 


ADD_EXECUTABLE(sandbox-build sandbox-build.cpp)
TARGET_LINK_LIBRARIES(sandbox-build ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES}) 
