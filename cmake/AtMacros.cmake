
function(at_prefix PREFIX IN_LIST)
  foreach(IN_FILE ${IN_LIST})
    list(APPEND OUT_LIST ${PREFIX}${IN_FILE})
  endforeach()
endfunction()

# ------------------
# Concatenate header files from a module
# @param IN_FILE file to be appended
# @param OUT_FILE the big joining file
# @param line_to_remove 
# ------------------
function(at_cat IN_FILE OUT_FILE lines_to_remove)
  file(READ ${IN_FILE} CONTENTS)
  # Remove gpl header
  string(REGEX REPLACE "/\\*\\*.*\\*\\*/(.*)" "\\1" CONTENTS2 "${CONTENTS}")
  foreach(line_to_remove ${lines_to_remove})
    string(REGEX REPLACE ${line_to_remove} "" CONTENTS2 "${CONTENTS2}")
  endforeach()
  file(APPEND ${OUT_FILE} "${CONTENTS2}")
endfunction()
# ------------------
# Set compilation standard to C11
# ------------------
macro(at_use_c11)
  if (CMAKE_VERSION VERSION_LESS "3.1")
    if (CMAKE_C_COMPILER_ID STREQUAL "GNU")
      set (CMAKE_C_FLAGS "--std=c11 ${CMAKE_C_FLAGS}")
    endif ()
  else ()
    set (CMAKE_C_STANDARD 11)
  endif ()
endmacro(at_use_c11)
# ------------------
# Create the tests for a module
# @param module the name of the module
# ------------------
macro(at_create_tests module)
  string(TOUPPER ${module} MODULEU)
  at_use_c11()
  IF(BUILD_${MODULEU})
    FOREACH(TEST ${AT_${MODULEU}_TESTS})
      add_executable       (test_${TEST} tests/${module}/test_${TEST}.c)
      target_link_libraries(test_${TEST} ${AT_${MODULEU}_LIBRARIES} cmocka)
      add_test             (test_${TEST} test_${TEST})
    ENDFOREACH()
  ENDIF()
endmacro()