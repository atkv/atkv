
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

# ------------------
# Download and build Cmocka
# ------------------
macro(at_download_cmocka)
  set(CMockaCMakeArgs
     -DCMAKE_BUILD_TYPE=${CMAKE_BUILD_TYPE}
     -DCMAKE_INSTALL_PREFIX=${CMAKE_SOURCE_DIR}/external
     -DCMAKE_C_COMPILER=${CMAKE_C_COMPILER}
     -DUNIT_TESTING=OFF
     )

  ExternalProject_Add(cmockaDownload
      PREFIX ${CMAKE_SOURCE_DIR}/external
      SOURCE_DIR ${CMAKE_SOURCE_DIR}/external/cmocka
      BINARY_DIR ${CMAKE_BINARY_DIR}/external/cmocka-build
      STAMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-stamp
      TMP_DIR ${CMAKE_BINARY_DIR}/external/cmocka-tmp
      INSTALL_DIR ${CMAKE_BINARY_DIR}/external
      CMAKE_ARGS ${CMockaCMakeArgs}
      # GIT_REPOSITORY "git://git.cryptomilk.org/projects/cmocka.git"
      #GIT_TAG "origin/master"
      #GIT_SUBMODULES ""
      UPDATE_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
      DOWNLOAD_COMMAND cd ${CMAKE_SOURCE_DIR} && git submodule update --init --recursive external/cmocka
  )
  set(CMOCKA_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
  set(CMOCKA_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
  add_library(cmocka SHARED IMPORTED)
  set_target_properties(cmocka PROPERTIES IMPORTED_LOCATION ${CMOCKA_LIB_DIR}/libcmocka.so)
endmacro()

macro(at_create_libraries AT_MODULES)
  foreach(AT_MODULE ${AT_MODULES})
    # Convert to uppercase
    string(TOUPPER ${AT_MODULE} AT_MODULE_UPPER)

    if(BUILD_${AT_MODULE_UPPER})
      file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/${AT_MODULE}.h.in "${AT_GPL_HEADER}")
      # Adding absolute path to headers
      foreach(AT_HDR ${AT_${AT_MODULE_UPPER}_HDRS})
        set(AT_HDR_ABS ${AT_INCLUDE_DIR}/at/${AT_MODULE}/${AT_HDR})
        set(lines_to_remove "#include <at/${AT_MODULE}.h>")
        list(APPEND AT_${AT_MODULE_UPPER}_HDRS_ABS ${AT_HDR_ABS})
        at_cat(${AT_HDR_ABS} ${CMAKE_CURRENT_BINARY_DIR}/${AT_MODULE}.h.in ${lines_to_remove})
      endforeach()
      # Adding absolute path to sources
      foreach(AT_SRC ${AT_${AT_MODULE_UPPER}_SRCS})
        list(APPEND AT_${AT_MODULE_UPPER}_SRCS_ABS ${AT_SOURCE_DIR}/${AT_MODULE}/${AT_SRC})
      endforeach()
      # Create one-header per module
      CONFIGURE_FILE(${CMAKE_CURRENT_BINARY_DIR}/${AT_MODULE}.h.in ${AT_INCLUDE_DIR}/at/${AT_MODULE}.h)
      # Get a list of all sources
      list(APPEND AT_SRCS ${AT_${AT_MODULE_UPPER}_SRCS_ABS})

      # Build the library
      add_library(at_${AT_MODULE} SHARED ${AT_${AT_MODULE_UPPER}_SRCS_ABS} ${AT_${AT_MODULE_UPPER}_HDRS_ABS})
    endif()

  endforeach()
endmacro()
