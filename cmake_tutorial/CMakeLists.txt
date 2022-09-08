cmake_minimum_required(VERSION 3.14)

# set the project name and version
project(Tutorial VERSION 2.0)

# specify the C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)


# make library optional 
option(USE_MYMATH "Use tutorial provided math implementation" ON)

# configure a header file to pass some of the CMake settings to the source code
configure_file(TutorialConfig.h.in TutorialConfig.hpp)

if(USE_MYMATH)
	# add the MathFunctions static library (library will get build)
	add_subdirectory(MathFunctions)
	# using EXTRA_* list-variables for includes, linkage
	list(APPEND EXTRA_LIBS MathFunctions)
	list(APPEND EXTRA_INCLUDES "${PROJECT_SOURCE_DIR}/MathFunctions")
endif()

# add the executables
add_executable(Tutorial tutorial.cpp)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# add build directory to the list pf paths to search for include files
# SHOULD BE PLACED AFTER add_executables
target_include_directories(Tutorial PUBLIC 
							"${PROJECT_BINARY_DIR}"
							"${EXTRA_INCLUDES}"
							)
