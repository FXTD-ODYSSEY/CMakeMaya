# NOTE compile hot_reload.mll
project(hot_reload)
file(REMOVE "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.mll")
if (NOT EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.mll)
    file(GLOB SRCS "maya_deformer/*.cpp" "maya_deformer/*.h")

    link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

    add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

    target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

    MAYA_PLUGIN(${PROJECT_NAME}) #runs a function with a PROJECT_NAME as an argument
endif()

# NOTE compile logic.dll
project(logic)

file(GLOB DLLS "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/*.dll")
file(REMOVE ${DLLS})
if (EXISTS ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.dll)
    file(RENAME "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}.dll" "${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/${PROJECT_NAME}_old.dll")
endif()

file(GLOB SRCS "logic/logic.cpp" "logic/logic.h")

link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents
