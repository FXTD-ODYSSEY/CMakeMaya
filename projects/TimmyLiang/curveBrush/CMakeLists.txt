
set(REPO_NAME "Maya-CurveBrush")
# TODO add noiseSphere name
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/${REPO_NAME})
set(SOURCE ${REPO_ROOT}/modules/${REPO_NAME}/source)
get_filename_component(CUREENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)

project(${CUREENT_FOLDER})
find_package(OpenGL REQUIRED) 

# locate source code folder
file(GLOB SRCS "${SOURCE}/*.cpp" "${SOURCE}/*.h")

include_directories(${MAYA_INCLUDE_DIR} ${OPENGL_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR} ) 
# message("OPENGL_LIBRARIES ${OPENGL_LIBRARIES}")
# message("OPENGL_LIBRARIES ${OPENGL_LIBRARIES}")
message("MAYA_LIBRARIES ${MAYA_LIBRARIES}")
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES} ${OPENGL_LIBRARIES}) 

MAYA_PLUGIN(${PROJECT_NAME})

