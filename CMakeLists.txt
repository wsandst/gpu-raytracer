###########################################################################
### Example CMake Setup demonstrating libraries and header-only projects" #
###########################################################################
# Includes a bunch of macros for improving IDE compatability, mainly focused on Windows Visual Studios"
# Supports a shaders/ folder in src/ as well as an assets/ folder in the repo, which are both copied/symlinked next to the exe
# Supports automatic inclusion of dll files next to the exe from dlls/ folder
# Only tested on Windows 10

### Settings ###
# Enables/disables symlinking the assets needed next to the generated exe in the IDE. Should work fine except for in Windows, where it only works with Admin Privilege
set(USING_SYMLINKS TRUE)
# If no symlinks are available, should we copy the assets next to the exe instead?
set(USING_COPY TRUE)

# Project setup
cmake_minimum_required(VERSION 3.17)

project(gpu-pathtracer
    LANGUAGES C CXX
    VERSION 1.0.0
    DESCRIPTION "GPU Compute Shader Pathtracer"
)

# Add cmake folder for custom find_package() files
list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_LIST_DIR}/cmake")

#For finding my preferred library location on Windows
list(APPEND CMAKE_PREFIX_PATH "C:/local")


### Add source files
# Add source files
file(GLOB_RECURSE SOURCE_FILES 
	${CMAKE_SOURCE_DIR}/src/*.c
	${CMAKE_SOURCE_DIR}/src/*.cpp)
	
# Add header files
file(GLOB_RECURSE HEADER_FILES 
	${CMAKE_SOURCE_DIR}/src/*.h
	${CMAKE_SOURCE_DIR}/src/*.hpp)
    
# Add GLSL shader files to the project if a shader folder exists
if (EXISTS ${PROJECT_SOURCE_DIR}/src/shaders)
    file(GLOB_RECURSE SHADER_FILES 
        ${CMAKE_SOURCE_DIR}/src/shaders/*.glsl
        ${CMAKE_SOURCE_DIR}/src/shaders/*.comp
        ${CMAKE_SOURCE_DIR}/src/shaders/*.frag
        ${CMAKE_SOURCE_DIR}/src/shaders/*.vert)
endif()
    
add_executable(${PROJECT_NAME} ${HEADER_FILES} ${SOURCE_FILES} ${SHADER_FILES})

### Packages ###
# SDL
find_package(SDL2 REQUIRED)

# GLM
find_package(GLM REQUIRED)

# GLEW
set(GLEW_USE_STATIC_LIBS ON)
find_package(GLEW REQUIRED)

find_package(OpenGL REQUIRED)

# Include files (eg header only libraries)
set(INCS 
    ${SDL2_INCLUDE_DIRS}
    ${GLM_INCLUDE_DIRS}
    ${GLEW_INCLUDE_DIRS}
    ${CMAKE_CURRENT_LIST_DIR}/external/stb
    ${CMAKE_CURRENT_LIST_DIR}/external/tinyobjloader
    )

include_directories(${INCS})

#Set variable LIBS to the libraries included
set(LIBS ${SDL2_LIBRARIES} ${GLM_LIBRARIES} GLEW::GLEW ${OPENGL_gl_LIBRARY})

#Link with libraries
target_link_libraries(${PROJECT_NAME} ${LIBS})
    
    
    
####################################################################################
### Macros for improving IDEs and such, should not be modified per project basis ###
####################################################################################
    
# Create virtual folders to make it look nicer in VS
if(MSVC_IDE)
    #Set working dir to be next to the exe
    set_property(TARGET ${PROJECT_NAME} PROPERTY VS_DEBUGGER_WORKING_DIRECTORY $<TARGET_FILE_DIR:${PROJECT_NAME}>)
    #Set the correct project as Startup
    set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT ${PROJECT_NAME})
	# Macro to preserve source files hierarchy in the IDE
	macro(GroupSources curdir)
		file(GLOB children RELATIVE ${PROJECT_SOURCE_DIR}/${curdir} ${PROJECT_SOURCE_DIR}/${curdir}/*)

		foreach(child ${children})
			if(IS_DIRECTORY ${PROJECT_SOURCE_DIR}/${curdir}/${child})
				GroupSources(${curdir}/${child})
			else()
				string(REPLACE "/" "\\" groupname ${curdir})
				string(REPLACE "src" "Sources" groupname ${groupname})
				source_group(${groupname} FILES ${PROJECT_SOURCE_DIR}/${curdir}/${child})
			endif()
		endforeach()
	endmacro()

	# Run macro
	GroupSources(src)
endif()

# Copy necessary dlls to VS
if(WIN32 AND EXISTS ${PROJECT_SOURCE_DIR}/dlls)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_directory
		"${PROJECT_SOURCE_DIR}/dlls"
		$<TARGET_FILE_DIR:${PROJECT_NAME}>
        COMMENT "Copying DLL files\n" VERBATIM)
endif()

#Generate symlinks for shaders/ and assets/, preferred option
if (${USING_SYMLINKS})
    # Add symlink for shaders to be next to the exe file.
    if (EXISTS ${PROJECT_SOURCE_DIR}/src/shaders)
        add_custom_command(
                 TARGET ${PROJECT_NAME} POST_BUILD
                 COMMAND ${CMAKE_COMMAND} -E create_symlink              
                 ${CMAKE_SOURCE_DIR}/src/shaders 
                 $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
                 COMMENT "Creating symlink to shader directory\n" VERBATIM)
    endif()
    if (EXISTS ${PROJECT_SOURCE_DIR}/assets)
        add_custom_command(
                 TARGET ${PROJECT_NAME} POST_BUILD
                 COMMAND ${CMAKE_COMMAND} -E create_symlink              
                 ${CMAKE_SOURCE_DIR}/assets
                 $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
                 COMMENT "Creating symlink to assets directory\n" VERBATIM)
    endif()
elseif(${USING_COPY})
    #Else copy over the files next to the exe file with every cmake.
    if (EXISTS ${PROJECT_SOURCE_DIR}/assets)
        #Copy over assets
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/assets
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>/assets
            COMMENT "Copying assets\n" VERBATIM)
    endif()
    if (EXISTS ${PROJECT_SOURCE_DIR}/src/shaders)
        #Copy over shaders
        add_custom_command(
            TARGET ${PROJECT_NAME} POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
                    ${CMAKE_SOURCE_DIR}/src/shaders
                    $<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders
            COMMENT "Copying shaders\n" VERBATIM)
    endif()
endif()