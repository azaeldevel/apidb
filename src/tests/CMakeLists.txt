CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(tests LANGUAGES CXX)

ADD_EXECUTABLE(testing-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR}.cpp ${SOURCES_BUILDING} )
TARGET_LINK_LIBRARIES(testing-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES})

##ADD_EXECUTABLE(sandbox-generator sandbox-generator.cpp)
##TARGET_LINK_LIBRARIES(sandbox-generator ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES})

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst)
#ADD_EXECUTABLE(sandbox-queries sandbox-queries.cpp ${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst/muposys.cpp)
#TARGET_LINK_LIBRARIES(sandbox-queries ${LIBRARIES}) 

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/muposys)
#ADD_EXECUTABLE(sandbox-siud sandbox-siud.cpp ${CMAKE_CURRENT_BINARY_DIR}/muposys/muposys.cpp)
#TARGET_LINK_LIBRARIES(sandbox-siud ${LIBRARIES}) 

#ADD_EXECUTABLE(sandbox-build sandbox-build.cpp)
#TARGET_LINK_LIBRARIES(sandbox-build ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES})

#include_directories(${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst)
#ADD_EXECUTABLE(sandbox-operations sandbox-operations.cpp )
#TARGET_LINK_LIBRARIES(sandbox-operations ${LIBRARIES} ${CMAKE_CURRENT_BINARY_DIR}/muposys-nlst/libmuposys.a)


#ADD_EXECUTABLE(Driver Driver.cpp)
#TARGET_LINK_LIBRARIES(Driver ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES})

#ADD_EXECUTABLE(ConfigureProject ConfigureProject.cpp)
#TARGET_LINK_LIBRARIES(ConfigureProject ${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME}-ConfigureProject-v${${CMAKE_PROJECT_NAME}_VERSION_MAJOR} ${LIBRARIES})

#add_test(NAME ConfigureProject-test COMMAND ConfigureProject)
#add_test(NAME Driver-test COMMAND Driver)

#add_test(NAME apidb-build-test COMMAND src/apidb-build -p src/tests/muposys -o src)

#add_test(NAME apidb-testing-generate COMMAND sandbox-generator)
