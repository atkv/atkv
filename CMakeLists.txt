cmake_minimum_required(VERSION 2.8)
project(atkv)
set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/")
# ===================================================================== OPTIONS
option(BUILD_CORE "" ON)
option(BUILD_GRAPH "" ON)
option(BUILD_IMGPROC "" ON)
option(BUILD_STUDIO "" ON)
option(BUILD_ML "" OFF)
option(BUILD_MEDICAL "" OFF)
option(BUILD_IFT "" ON)
option(BUILD_CHART "" OFF)
option(BUILD_GL "" OFF)
option(BUILD_TESTING "" ON)
option(BUILD_DOCS    "Build doxygen documentation" OFF)
option(COVERALLS     "Generate coveralls data" OFF)
option(COVERALLS_UPLOAD "Upload the generated coveralls json" ON)
#================================================== CUSTOM FUNCTIONS AND MACROS
include(cmake/AtMacros.cmake)
set(EXTERNAL_INCLUDE_DIR ${CMAKE_SOURCE_DIR}/external/include)
set(EXTERNAL_LIB_DIR ${CMAKE_SOURCE_DIR}/external/lib)
include_directories(${EXTERNAL_INCLUDE_DIR} include)
#================================================================= DEPENDENCIES
find_package(PkgConfig REQUIRED)
# ------------------
# GObject
# ------------------
pkg_check_modules(GOBJECT REQUIRED gobject-2.0)
INCLUDE_DIRECTORIES(${GOBJECT_INCLUDE_DIRS})
add_definitions(-DHAVE_GOBJECT)

# ------------------
# GTK 3
# ------------------
if(BUILD_STUDIO)
  pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
  include_directories(${GTK3_INCLUDE_DIRS})
endif()

# ------------------
# Zlib
# ------------------
if(BUILD_IMGPROC)
    pkg_check_modules(ZLIB REQUIRED zlib)
    INCLUDE_DIRECTORIES(${ZLIB_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_ZLIB)
endif()

# ------------------
# JPEG
# ------------------
if(BUILD_IMGPROC)
    find_package(JPEG QUIET)
    INCLUDE_DIRECTORIES(${JPEG_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_JPEG)
endif()

# ------------------
# PNG
# ------------------
if(BUILD_IMGPROC)
    find_package(PNG QUIET)
    INCLUDE_DIRECTORIES(${PNG_INCLUDE_DIRS})
    ADD_DEFINITIONS(-DHAVE_PNG)
endif()

# ===================================================================== MODULES
set(AT_MODULES chart core graph gl ift imgproc medical ml)
set(AT_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(AT_SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)
FOREACH(AT_MODULE ${AT_MODULES})
  STRING(TOUPPER ${AT_MODULE} AT_MODULE_UPPER)
  SET(AT_${AT_MODULE_UPPER}_INCLUDE_DIR ${AT_INCLUDE_DIR}/at/${AT_MODULE})
  SET(AT_${AT_MODULE_UPPER}_SOURCE_DIR ${AT_SOURCE_DIR}/${AT_MODULE})
ENDFOREACH()
set(AT_VERSION_MAJOR 0)
set(AT_VERSION_MINOR 0)
set(AT_VERSION_PATCH 1)
set(AT_VERSION_STRING ${AT_VERSION_MAJOR}.${AT_VERSION_MINOR}.${AT_VERSION_PATCH})
set(AT_INSTALL_PATH "\"${CMAKE_INSTALL_PREFIX}\"")
set(AT_RESOURCE_PATH "\"${CMAKE_INSTALL_PREFIX}/share/at\"")


FILE(READ "GPLHEADER" AT_GPL_HEADER)
set(CMAKE_BUILD_TYPE Debug)

set(AT_TYPES uint8_t uint16_t uint32_t uint64_t
              int8_t  int16_t  int32_t  int64_t
              float  double)
set(AT_VEC_TYPES 3 4 5 6 7)
set(AT_TYPES_ENUM UINT8 UINT16 UINT32 UINT64
                   INT8  INT16  INT32  INT64
                  FLOAT DOUBLE)
set(AT_TYPES_FLOAT FALSE FALSE FALSE FALSE
                   FALSE FALSE FALSE FALSE
                   TRUE TRUE)
set(AT_TYPES_MAX UINT8_MAX UINT16_MAX UINT32_MAX UINT64_MAX
                  INT8_MAX  INT16_MAX  INT32_MAX  INT64_MAX
                   FLT_MAX    DBL_MAX)
set(AT_TYPES_MIN         0          0          0          0
                  INT8_MIN  INT16_MIN  INT32_MIN  INT64_MIN
                   FLT_MIN    DBL_MIN)

# CORE
# ----------------
list(LENGTH AT_TYPES len1)
math(EXPR   len2 "${len1} - 1")
# Array
foreach(TYPE_INDEX RANGE ${len2})
  list(GET AT_TYPES      ${TYPE_INDEX} AT_TYPE)
  list(GET AT_TYPES_ENUM ${TYPE_INDEX} AT_TYPE_ENUM)
  list(GET AT_TYPES_MAX  ${TYPE_INDEX} AT_TYPE_MAX)
  list(GET AT_TYPES_MIN  ${TYPE_INDEX} AT_TYPE_MIN)
  list(GET AT_TYPES_FLOAT ${TYPE_INDEX} AT_TYPE_FLOAT)
  list(APPEND AT_ARRAY_TYPES_HDRS array_${AT_TYPE}.h)
  list(APPEND AT_ARRAY_TYPES_SRCS array_${AT_TYPE}.c)
  string(TOUPPER ${AT_TYPE} AT_TYPE_UPPER)
  configure_file(${AT_INCLUDE_DIR}/at/core/array_template.h.in ${AT_INCLUDE_DIR}/at/core/array_${AT_TYPE}.h)
  configure_file(${AT_SOURCE_DIR}/core/array_template.c.in ${AT_SOURCE_DIR}/core/array_${AT_TYPE}.c)
endforeach()
# Vec
foreach(AT_VEC_TYPE ${AT_VEC_TYPES})
  list(APPEND AT_VEC_TYPES_HDRS vec${AT_VEC_TYPE}.h)
  list(APPEND AT_VEC_TYPES_SRCS vec${AT_VEC_TYPE}.c)
  configure_file(${AT_INCLUDE_DIR}/at/core/vec_template.h.in ${AT_INCLUDE_DIR}/at/core/vec${AT_VEC_TYPE}.h)
  configure_file(${AT_SOURCE_DIR}/core/vec_template.c.in ${AT_SOURCE_DIR}/core/vec${AT_VEC_TYPE}.c)
endforeach()

set(AT_CORE_HDRS type.h range.h macro.h config.h ${AT_VEC_TYPES_HDRS} mat4.h arraybase.h
    ${AT_ARRAY_TYPES_HDRS} array.h bucketqueue.h)
set(AT_CORE_SRCS config.c range.c ${AT_VEC_TYPES_SRCS} mat4.c arraybase.c ${AT_ARRAY_TYPES_SRCS} bucketqueue.c)
set(AT_CORE_TESTS array vec3 vec4 vec6 vec7 mat4 bucketqueue)
set(AT_CORE_LIBRARIES at_core m ${GOBJECT_LIBRARIES})
CONFIGURE_FILE("${AT_CORE_INCLUDE_DIR}/config.h.in"
               "${AT_CORE_INCLUDE_DIR}/config.h")

# STUDIO
# ----------------
set(AT_STUDIO_HDRS app.h window.h)
set(AT_STUDIO_SRCS app.c window.c main.c)
set(AT_STUDIO_LIBRARIES ${AT_CORE_LIBRARIES} ${GTK3_LIBRARIES})

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
set(AT_IMGPROC_HDRS znzfile.h nifti.h dicom.h image_io.h drawing.h)
set(AT_IMGPROC_SRCS znzfile.c nifti.c dicom.c image_io.c drawing.c)
set(AT_IMGPROC_LIBRARIES at_imgproc ${AT_CORE_LIBRARIES} ${ZLIB_LIBRARIES} ${PNG_LIBRARIES} ${JPEG_LIBRARIES})
set(AT_IMGPROC_TESTS nifti dicom image_io)


# GRAPH
# ----------------
set(AT_GRAPH_HDRS adjacency.h grapharray.h component.h)
set(AT_GRAPH_SRCS adjacency.c grapharray.c component.c)
set(AT_GRAPH_LIBRARIES at_graph ${AT_CORE_LIBRARIES})
set(AT_GRAPH_TESTS component)

# IFT
# ----------------
set(AT_IFT_HDRS ift.h)
set(AT_IFT_SRCS ift.c)
set(AT_IFT_LIBRARIES at_ift ${AT_GRAPH_LIBRARIES})
set(AT_IFT_TESTS ift)


# Compiling modules
at_create_libraries("${AT_MODULES}")

# Studio Executable
if(BUILD_STUDIO)
  foreach(AT_HDR ${AT_STUDIO_HDRS})
    list(APPEND AT_STUDIO_HDRS_ABS ${AT_INCLUDE_DIR}/at/studio/${AT_HDR})
  endforeach()
  foreach(AT_SRC ${AT_STUDIO_SRCS})
    list(APPEND AT_STUDIO_SRCS_ABS ${AT_SOURCE_DIR}/studio/${AT_SRC})
  endforeach()
  add_executable(at_studio ${AT_STUDIO_HDRS_ABS} ${AT_STUDIO_SRCS_ABS})
  target_link_libraries(at_studio ${GTK3_LIBRARIES})
endif()

# ======================================================================= TESTS
if(BUILD_TESTING)
  include(CTest)
  include(Coveralls)
  include(ExternalProject)
  find_package(CMocka QUIET)
  if(NOT CMOCKA_FOUND)
    at_download_cmocka()
  endif()
  ENABLE_TESTING()
  # profiling directives (GCov and GProf)
  #ADD_DEFINITIONS(-fprofile-arcs -ftest-coverage -pg)
  #set(CMAKE_EXE_LINKER_FLAGS "-g -Wall -std=c11 -Wshadow -Wunused-variable -Wno-unused-function -Wunused -Wno-system-headers -pg -fprofile-arcs -ftest-coverage")

  # Setup coveralls
  IF(COVERALLS)
    coveralls_setup("${AT_SRCS}" ${COVERALLS_UPLOAD} "${CMAKE_SOURCE_DIR}/cmake/")
  ENDIF()

  # Create tests for each module
  FOREACH(AT_MODULE ${AT_MODULES})
    AT_CREATE_TESTS(${AT_MODULE})
  ENDFOREACH()
  Message(STATUS "Tests compilation configured...")
endif(BUILD_TESTING)

# ======================================================================== DOCS
