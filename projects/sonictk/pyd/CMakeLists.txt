# #plugin name
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/pyd)

project(py_hello) #project name

file(GLOB SRCS "pyHello/*.cpp" "pyHello/*.h")

include_directories(${MAYA_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR}) #define a list of preprocessor include file search directories
link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

set(EXPORT_FLAGS "/export:initpy_hello")
if (${MAYA_VERSION} GREATER 2020)
    set(EXPORT_FLAGS "/export:PyInit_py_hello")
endif()

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

# output suffix as pyd format
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${EXPORT_FLAGS}
    SUFFIX ".pyd"
)

project(py_deformer) #project name

file(GLOB SRCS "pyDeformer/*.cpp" "pyDeformer/*.h")
include_directories(${MAYA_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR}) #define a list of preprocessor include file search directories
link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

set(EXPORT_FLAGS "/export:initpy_deformer")
if (${MAYA_VERSION} GREATER 2020)
    set(EXPORT_FLAGS "/export:PyInit_py_deformer")
endif()
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

# output suffix as pyd format
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${EXPORT_FLAGS}
    SUFFIX ".pyd"
)


project(py_command) #project name

file(GLOB SRCS "pyCommand/*.cpp" "pyCommand/*.h")
include_directories(${MAYA_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR}) #define a list of preprocessor include file search directories
link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

set(EXPORT_FLAGS "/export:initpy_command")
if (${MAYA_VERSION} GREATER 2020)
    set(EXPORT_FLAGS "/export:PyInit_py_command /export:initializePlugin /export:uninitializePlugin")
endif()
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES})

# output suffix as pyd format
set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${EXPORT_FLAGS}
    SUFFIX ".pyd"
)


#plugin name
project(pyMayaPlugins) #project name

file(GLOB SRCS "pyMayaPlugins/*.cpp" "pyMayaPlugins/*.h")

include_directories(${MAYA_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR}) #define a list of preprocessor include file search directories

link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

MAYA_PLUGIN(${PROJECT_NAME})

#plugin name
find_package(Pybind11 REQUIRED) 

project(pybind11cpp) #project name


file(GLOB SRCS "pybind11/*.cpp" "pybind11/*.h")

include_directories(${MAYA_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR} ${PYBIND11_INCLUDE_DIR})

link_directories(${MAYA_LIBRARY_DIR}) #specifies a directory where a linker should search for libraries

add_library(${PROJECT_NAME} SHARED ${SRCS}) #Add a dynamic library to the project using the specified source files

# pybind11_add_module(${PROJECT_NAME} ${SRCS})

target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) #specifies list of libraries to use when linking the terget and its dependents

if(${MAYA_VERSION} GREATER 2020) 
    set(PYBIND_LINK_FLAGS "/export:PyInit_pybind11cpp")
else() 
    set(PYBIND_LINK_FLAGS "/export:initpybind11cpp")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${PYBIND_LINK_FLAGS}
    SUFFIX ".pyd"
)
