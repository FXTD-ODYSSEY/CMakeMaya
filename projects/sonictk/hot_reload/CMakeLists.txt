# NOTE compile hot_reload.mll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/hot_reload)

project(hot_reload)
file(REMOVE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.mll")
if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.mll)
    file(GLOB SRCS "maya_deformer/*.cpp" "maya_deformer/*.h")


    include_directories(${MAYA_INCLUDE_DIR}) 
    link_directories(${MAYA_LIBRARY_DIR}) 
    add_library(${PROJECT_NAME} SHARED ${SRCS}) 
    target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 
    MAYA_PLUGIN(${PROJECT_NAME})
endif()

# NOTE compile logic.dll
project(logic)

file(GLOB DLLS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/*.dll")
if (NOT DLLS STREQUAL "")
file(REMOVE ${DLLS})
endif()

if (EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.dll)
    file(RENAME "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}_old.dll")
endif()

file(GLOB SRCS "logic/*.cpp" "logic/*.h")

include_directories(${MAYA_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS})
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})
