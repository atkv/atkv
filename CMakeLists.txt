cmake_minimum_required(VERSION 2.8)
project(atkv)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# ===================================================================== OPTIONS
option(BUILD_CORE "" ON)
option(BUILD_STUDIO "" ON)
option(BUILD_ML "" OFF)
option(BUILD_MEDICAL "" OFF)
option(BUILD_IFT "" OFF)
option(BUILD_CHART "" OFF)
option(BUILD_GL "" OFF)

#================================================== CUSTOM FUNCTIONS AND MACROS
include(cmake/AtMacros.cmake)

#================================================================= DEPENDENCIES
find_package(PkgConfig REQUIRED)
# GTK 3
# ----------------
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
include_directories(${GTK3_INCLUDE_DIRS})

# ===================================================================== MODULES
set(AT_MODULES chart core gl ift imgproc medical ml)
set(AT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(AT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

# CORE
# ----------------
set(AT_CORE_HDRS config.h array.h)
set(AT_CORE_SRCS array.c)

# STUDIO
# ----------------
set(AT_STUDIO_HDRS app.h window.h)
set(AT_STUDIO_SRCS app.c window.c main.c)

# ML
# ----------------
set(AT_ML_HDRS app.h window.h)
set(AT_ML_SRCS app.c window.c)

# MEDICAL
# ----------------
set(AT_MEDICAL_HDRS app.h window.h)
set(AT_MEDICAL_SRCS app.c window.c)

# IMGPROC
# ----------------
set(AT_IMGPROC_HDRS app.h window.h)
set(AT_IMGPROC_SRCS app.c window.c)

# Compiling modules
foreach(AT_MODULE ${AT_MODULES})
  # Convert to uppercase
  string(TOUPPER ${AT_MODULE} AT_MODULE_UPPER)
  
  if(BUILD_${AT_MODULE_UPPER})
    # Adding absolute path to headers
    foreach(AT_HDR ${AT_${AT_MODULE_UPPER}_HDRS})
      message(${AT_HDR})
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

    # Build the library
    add_library(at_${AT_MODULE} SHARED ${AT_${AT_MODULE_UPPER}_SRCS_ABS} ${AT_${AT_MODULE_UPPER}_HDRS_ABS})
  endif()
  
endforeach()

# Studio Executable
foreach(AT_HDR ${AT_STUDIO_HDRS})
  list(APPEND AT_STUDIO_HDRS_ABS ${AT_INCLUDE_DIR}/at/studio/${AT_HDR})
endforeach()
foreach(AT_SRC ${AT_STUDIO_SRCS})
  list(APPEND AT_STUDIO_SRCS_ABS ${AT_SOURCE_DIR}/studio/${AT_SRC})
endforeach()
message(STATUS ${AT_STUDIO_SRCS_ABS})

add_executable(at_studio ${AT_STUDIO_HDRS_ABS} ${AT_STUDIO_SRCS_ABS})
target_link_libraries(at_studio ${GTK3_LIBRARIES})

# ======================================================================= TESTS
# ======================================================================== DOCS