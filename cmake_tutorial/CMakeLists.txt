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

# define can be passed via cli command to configure: cmake ./build -DUSE_MYMATH=ON(OFF)
if(USE_MYMATH)
	# add the MathFunctions static library (library will get build)
	add_subdirectory(MathFunctions)
	# using EXTRA_* list-variables for includes, linkage
	list(APPEND EXTRA_LIBS MathFunctions)
endif()

# add the executables
add_executable(Tutorial tutorial.cpp)

target_link_libraries(Tutorial PUBLIC ${EXTRA_LIBS})

# add build directory to the list pf paths to search for include files
# SHOULD BE PLACED AFTER add_executables
target_include_directories(Tutorial PUBLIC 
							"${PROJECT_BINARY_DIR}"
							)


# Install rules:
# for the application we want to install the executable and configured header
# command: cmake install

# The CMake variable CMAKE_INSTALL_PREFIX is used to determine the root of where 
# the files will be installed. If using the cmake --install command, the installation 
# prefix can be overridden via the --prefix argument. For example:
# cmake --install ./build --prefix "/home/myuser/installdir"

install(TARGETS Tutorial DESTINATION bin)
install(FILES "${PROJECT_BINARY_DIR}/TutorialConfig.h" DESTINATION include)


# Testing:
# Rebuild the application and then cd to the binary directory and run the ctest executable: 
# ctest -N and ctest -VV.
# enable_testing()

# enable dashboard scripting
include(CTest)

# does the application run
add_test(NAME Runs COMMAND Tutorial 25)

# does the usage message work?
add_test(NAME Usage COMMAND Tutorial)
set_tests_properties(Usage PROPERTIES PASS_REGULAR_EXPRESSION "Usage:.*number")

# define a function to simplify adding tests
function(do_test target arg result)
  add_test(NAME Comp${arg} COMMAND ${target} ${arg})
  set_tests_properties(Comp${arg} PROPERTIES PASS_REGULAR_EXPRESSION ${result})
endfunction()

# do a bunch of result based tests
do_test(Tutorial 4 "4 is 2")
do_test(Tutorial 9 "9 is 3")
do_test(Tutorial 5 "5 is 2.236")
do_test(Tutorial 7 "7 is 2.645")
do_test(Tutorial 25 "25 is 5")
do_test(Tutorial -25 "-25 is (-nan|nan|0)")
do_test(Tutorial 0.0001 "0.0001 is 0.01")