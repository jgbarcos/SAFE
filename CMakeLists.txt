cmake_minimum_required(VERSION 2.6)
project(dev0)
		
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_SOURCE_DIR}/cmake")
		   
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/lib)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# Set up c++11
include(CheckCXXCompilerFlag)
CHECK_CXX_COMPILER_FLAG("-std=c++14" COMPILER_SUPPORTS_CXX14)
CHECK_CXX_COMPILER_FLAG("-std=c++1y" COMPILER_SUPPORTS_CXX1y)
if(COMPILER_SUPPORTS_CXX14)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14")
    
elseif(COMPILER_SUPPORTS_CXX1Y)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++1y")
else()
        message(STATUS "The compiler ${CMAKE_CXX_COMPILER} has no C++14 support. Please use a different C++ compiler.")
endif()

# Set up compiler
if (${CMAKE_CXX_COMPILER_ID} STREQUAL "GNU" OR ${CMAKE_CXX_COMPILER_ID} STREQUAL "Clang")
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wno-unused-parameter -pedantic")
	set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_DEBUG} -g")
	set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS} ${CMAKE_CXX_FLAGS_RELEASE} -O2")
                             
elseif (${CMAKE_CXX_COMPILER_ID} STREQUAL "MSVC")
	if (CMAKE_CXX_FLAGS MATCHES "/W[0-4]")
		string(REGEX REPLACE "/W[0-4]" "/W4" CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS}")
	else()
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
	endif()
endif()

# Look up SDL2 and add the include directory to our include path
find_package(SDL2 REQUIRED)
find_package(SDL2_image REQUIRED)
find_package(SDL2_ttf REQUIRED)
find_package(SDL2_gfx REQUIRED)
find_package(Box2D REQUIRED)
find_package(Lua REQUIRED)

include_directories(${SDL2_INCLUDE_DIR}
                    ${SDL2_IMAGE_INCLUDE_DIR}
                    ${SDL2_TTF_INCLUDE_DIR}
                    ${SDL2GFX_INCLUDE_DIR}
                    ${BOX2D_INCLUDE_DIR}
                    ${LUA_INCLUDE_DIR}
                    )
# Documentation
option(BUILD_DOCUMENTATION
    "Create and install the HTML based API documentation (requires Doxygen)" OFF)
IF(BUILD_DOCUMENTATION)
  FIND_PACKAGE(Doxygen)
  IF(NOT DOXYGEN_FOUND)
    MESSAGE(FATAL_ERROR "Doxygen is needed to build the documentation.")
  ENDIF()

  SET( doxyfile_in          ${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in)
  SET( doxyfile             ${PROJECT_BINARY_DIR}/Doxyfile)
  SET( doxy_html_index_file ${CMAKE_CURRENT_BINARY_DIR}/html/index.html)
  SET( doxy_output_root     ${CMAKE_CURRENT_BINARY_DIR}) # Pasted into Doxyfile.in
  SET( doxy_input           ${PROJECT_SOURCE_DIR}/src) # Pasted into Doxyfile.in
  SET( doxy_extra_files     ${CMAKE_CURRENT_SOURCE_DIR}/mainpage.dox) # Pasted into Doxyfile.in

  CONFIGURE_FILE( ${doxyfile_in} ${doxyfile} @ONLY )

  ADD_CUSTOM_COMMAND(
    OUTPUT ${doxy_html_index_file}
    COMMAND ${DOXYGEN_EXECUTABLE} ${doxyfile}
    # The following should be ${doxyfile} only but it
    # will break the dependency.
    # The optimal solution would be creating a
    # custom_command for ${doxyfile} generation
    # but I still have to figure out how...
    MAIN_DEPENDENCY ${doxyfile} ${doxyfile_in}
    DEPENDS project_targets ${doxy_extra_files}
    COMMENT "Generating HTML documentation"
  )

  ADD_CUSTOM_TARGET( doc ALL DEPENDS ${doxy_html_index_file} )

  INSTALL( DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/html DESTINATION share/doc )
ENDIF()               

# Project cmake configuration
file(GLOB PROJECT_SRC
    "src/*.cpp"
    "src/SAFE/*.cpp"
)

file(GLOB PROJECT_HEADERS
    "include/*.h"
    "include/SAFE/*.h"
)

include_directories(include)
add_executable(${PROJECT_NAME} ${PROJECT_SRC} ${PROJECT_HEADERS})
target_link_libraries(${PROJECT_NAME} ${SDL2_LIBRARY}
                             ${SDL2_IMAGE_LIBRARIES}
                             ${SDL2_TTF_LIBRARIES}
                             ${SDL2GFX_LIBRARY}
                             ${BOX2D_LIBRARY}
                             ${LUA_LIBRARIES})

message ( "Used Flags: " ${CMAKE_CXX_FLAGS} )
                             
#install(TARGETS lab0 RUNTIME DESTINATION ${BIN_DIR})
