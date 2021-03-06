cmake_minimum_required(VERSION 3.4)
project(Test)
include( ../externals/CMakeUtils/CMakeUtils.txt)

set(CMAKE_CXX_STANDARD 14)

if( IS_WINDOWS )
	# Build at warning level 4 on Windows.
	# Treat all warnings as errors
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /WX")
	# Setup path for boost libs
	link_directories("C:/Boost/lib")
else()
	# Build with all warnings
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Werror")
	if (NOT IS_OSX)
		set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread")
	endif ()
	# Setup path for boost libs
	if (${MINGW})
		link_directories("c:/Boost/lib/")
	else ()
		link_directories("/usr/local/boost/lib")
	endif ()
endif()

include_directories(../lout/dist/include)

set( SOURCE_FILES
		test.cpp
		TestOutput.cpp
		TestOutput.h)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
target_link_libraries(${PROJECT_NAME} lout)

# Output final binaries in the dist folder so they easily can be distributed together with the header files.
set_target_properties(${PROJECT_NAME}
		PROPERTIES
		ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		LIBRARY_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		RUNTIME_OUTPUT_DIRECTORY "${CMAKE_CURRENT_LIST_DIR}/dist/bin/${CMAKE_BUILD_TYPE}"
		)
