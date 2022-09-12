# build as static library
# let CMake know that mysqrt.cxx depends on the generated file Table.h
add_library(MathFunctions mysqrt.cpp ${CMAKE_CURRENT_BINARY_DIR}/Table.h)

# INTERFACE means things that consumers require but the producer doesn't.
# Consumers - those who includes MathFunctions library .
target_include_directories(MathFunctions
          INTERFACE ${CMAKE_CURRENT_SOURCE_DIR}
          PRIVATE ${CMAKE_CURRENT_BINARY_DIR}
          )

# Adding System Introspection
# does this system provide the log and exp functions?
include(CheckCXXSourceCompiles)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::log(1.0);
    return 0;
  }
" HAVE_LOG)
check_cxx_source_compiles("
  #include <cmath>
  int main() {
    std::exp(1.0);
    return 0;
  }
" HAVE_EXP)

# If available, use target_compile_definitions() to specify 
# HAVE_LOG and HAVE_EXP as PRIVATE compile definitions.
if(HAVE_LOG AND HAVE_EXP)
  target_compile_definitions(MathFunctions
                             PRIVATE "HAVE_LOG" "HAVE_EXP")
endif()


# Custom command adding - generating file while building
add_executable(MakeTable MakeTable.cxx)

add_custom_command(
  OUTPUT "${CMAKE_CURRENT_BINARY_DIR}/Table.h"
  COMMAND MakeTable ${CMAKE_CURRENT_BINARY_DIR}/Table.h
  DEPENDS MakeTable
  )



# Install rules:
# for MathFunctions we want to install the library and header file 
install(TARGETS MathFunctions DESTINATION lib)
install(FILES MathFunctions.h DESTINATION include)
