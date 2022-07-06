

get_filename_component(CUREENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/${CUREENT_FOLDER})
set(SOURCE ${REPO_ROOT}/modules/${CUREENT_FOLDER}/src)

project(cvwrap)

# locate source code folder
file(GLOB SRCS "${SOURCE}/*.cpp" "${SOURCE}/*.h" "${SOURCE}/*.cl")

include_directories(${MAYA_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 

if (WIN32)
    set(COMPILE_FLAGS "/arch:AVX")
else()
    set(COMPILE_FLAGS "-mavx")
endif()
target_compile_options(${PROJECT_NAME} PRIVATE ${COMPILE_FLAGS})

MAYA_PLUGIN(${PROJECT_NAME})

