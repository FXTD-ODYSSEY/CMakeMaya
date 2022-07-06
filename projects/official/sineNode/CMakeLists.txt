#plugin name
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION})

project(sineNode)

file(GLOB SRCS "*.cpp" "*.h")

include_directories(${MAYA_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 

MAYA_PLUGIN(${PROJECT_NAME})
