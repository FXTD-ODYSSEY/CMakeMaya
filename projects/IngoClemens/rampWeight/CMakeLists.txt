

get_filename_component(CUREENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/${CUREENT_FOLDER})
set(SOURCE ${REPO_ROOT}/modules/${CUREENT_FOLDER}/source)

project(rampWeights)

# locate source code folder
file(GLOB SRCS "${SOURCE}/${PROJECT_NAME}/*.cpp" "${SOURCE}/${PROJECT_NAME}/*.h")

include_directories(${MAYA_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 

MAYA_PLUGIN(${PROJECT_NAME})

project(weightsServer)

# locate source code folder
file(GLOB SRCS "${SOURCE}/${PROJECT_NAME}/*.cpp" "${SOURCE}/${PROJECT_NAME}/*.h")

include_directories(${MAYA_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 

MAYA_PLUGIN(${PROJECT_NAME})
