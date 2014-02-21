cmake_minimum_required(VERSION 2.6)

project(ObjetEtTracking)

find_package ( OpenCV )
find_package ( OpenGL )
find_package ( GLUT )

# sources extension
set(SRCEXT "cpp")

# add everything for aruco
include_directories( /usr/local/include )

# macro used to link osg libraries
#macro( config_project LIBNAME )
# include_directories( ${${LIBNAME}_INCLUDE_DIR} )
# target_link_libraries( ${CMAKE_PROJECT_NAME} ${${LIBNAME}_LIBRARY} )
#endmacro()

# searching for source files : files in the SRCDIR directory with the SRCEXT extension.
file(
    GLOB
    source_files
    *.${SRCEXT}
)

# create executable
add_executable(
    ${CMAKE_PROJECT_NAME}
    ${source_files}
)


target_link_libraries(
	ObjetEtTracking
	aruco
    ${OPENGL_LIBRARIES}
    ${GLUT_LIBRARIES}
    ${OpenCV_LIBS}
)
