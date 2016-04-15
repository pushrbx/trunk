cmake_minimum_required(VERSION 3.0)

option( INSTALL_QPOSES_PLUGIN "Check to install qPoses plugin" OFF )

# CloudCompare 'qPoses' plugin
if (INSTALL_QPOSES_PLUGIN)
    project( QPOSES_PLUGIN )
    
    set(Boost_NO_SYSTEM_PATHS TRUE)
	if (Boost_NO_SYSTEM_PATHS)
	    set(BOOST_ROOT "C:/local/boost_1_59_0")
	    set(Boost_DIR "C:/local/boost_1_59_0")
	    set(BOOST_INCLUDE_DIR "${Boost_DIR}")
	    set(BOOST_LIBRARY_DIR "${Boost_DIR}/lib64-msvc-12.0")
	    message(STATUS "Boost path has been set.")
	endif (Boost_NO_SYSTEM_PATHS)
	
    find_package(Boost REQUIRED regex date_time system filesystem thread)
	include_directories(${BOOST_INCLUDE_DIR})
    #load necessary libraries (see qPCV for an example)
    #add_subdirectory (LIB1)
    
    #if the plugin is an 'OpenGL filter', uncomment the line below
    #set( CC_OPENGL_FILTER ON BOOL)
    include( ../CMakePluginTpl.cmake )
    
    #set dependencies to necessary libraries (see qPCV for an example)
    #target_link_libraries( ${PROJECT_NAME} LIB1 )
    #include_directories( ${LIB1_INCLUDE_DIR} )
endif()
