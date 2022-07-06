#plugin name
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/pyd/maya${MAYA_VERSION})

if (${MAYA_VERSION} LESS 2020) 
set(PYTHON_INCLUDE_DIR ${MAYA_INCLUDE_DIR}/python2.7)
else() 
set(PYTHON_INCLUDE_DIR ${MAYA_INCLUDE_DIR}/Python)
endif()

project(py_hello) #project name

file(GLOB SRCS "pyHello/*.cpp" "pyHello/*.h")

#as we've already loaded FindMaya.cmake with find_package() command
#we now have an access to all variables that we defined in this module

include_directories(${MAYA_INCLUDE_DIR}) #define a list of preprocessor include file search directories
include_directories(${PYTHON_INCLUDE_DIR})
link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

# output suffix as pyd format
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS "/export:initpy_hello"
    SUFFIX ".pyd"
)


#plugin name
project(pyMayaPlugins) #project name

file(GLOB SRCS "pyMayaPlugins/*.cpp" "pyMayaPlugins/*.h")

#as we've already loaded FindMaya.cmake with find_package() command
#we now have an access to all variables that we defined in this module

include_directories(${MAYA_INCLUDE_DIR}) #define a list of preprocessor include file search directories
include_directories(${PYTHON_INCLUDE_DIR})

link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

MAYA_PLUGIN(${PROJECT_NAME})
