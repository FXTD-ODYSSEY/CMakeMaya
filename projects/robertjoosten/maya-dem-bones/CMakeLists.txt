
get_filename_component(CUREENT_FOLDER ${CMAKE_CURRENT_SOURCE_DIR} NAME)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_RUNTIME_OUTPUT_DIRECTORY_RELEASE}/maya${MAYA_VERSION}/${CUREENT_FOLDER})
set(SOURCE ${REPO_ROOT}/modules/${CUREENT_FOLDER}/src)

project(_dem_bones)

find_package(Pybind11 REQUIRED) 

# locate source code folder
file(GLOB SRCS "${SOURCE}/*.cpp" "${SOURCE}/*.h")

include_directories(${MAYA_INCLUDE_DIR} ${DEMBONES_INCLUDE_DIR}  ${EIGEN_INCLUDE_DIR} ${PYBIND11_INCLUDE_DIR} ${MAYA_PYTHON_INCLUDE_DIR}) 
link_directories(${MAYA_LIBRARY_DIR}) 
add_library(${PROJECT_NAME} SHARED ${SRCS}) 
target_link_libraries(${PROJECT_NAME} ${MAYA_LIBRARIES}) 

if(${MAYA_VERSION} GREATER 2020) 
    set(PYBIND_LINK_FLAGS "/export:PyInit__dem_bones")
else() 
    set(PYBIND_LINK_FLAGS "/export:init_dem_bones")
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES
    LINK_FLAGS ${PYBIND_LINK_FLAGS}
    SUFFIX ".pyd"
)

