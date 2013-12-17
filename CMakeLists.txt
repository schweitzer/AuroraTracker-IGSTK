CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
IF(COMMAND CMAKE_POLICY)
  CMAKE_POLICY(SET CMP0003 NEW)
ENDIF(COMMAND CMAKE_POLICY)

PROJECT( IGSTK-AURORATRACKER )

# version
SET(IGSTK_VERSION_MAJOR "0")
SET(IGSTK_VERSION_MINOR "1")
SET(IGSTK_VERSION_PATCH "0")

SET (LIBRARY_OUTPUT_PATH ${IGSTK_BINARY_DIR}/bin CACHE PATH "Single output directory for building all libraries." FORCE)
SET (EXECUTABLE_OUTPUT_PATH ${IGSTK_BINARY_DIR}/bin CACHE PATH "Single output directory for building all executables." FORCE)

# Append the library version information to the library target properties.
OPTION(IGSTK_WITH_LIBRARY_VERSION "Build with library version information" OFF)
IF(IGSTK_WITH_LIBRARY_VERSION)
  SET(IGSTK_LIBRARY_PROPERTIES ${IGSTK_LIBRARY_PROPERTIES}
#   BUILD_WITH_INSTALL_RPATH OFF
#   INSTALL_RPATH ${CMAKE_INSTALL_PREFIX}/lib
#   INSTALL_NAME_DIR ${CMAKE_INSTALL_PREFIX}/lib
    VERSION ${IGSTK_VERSION_MAJOR}.${IGSTK_VERSION_MINOR}.${IGSTK_VERSION_PATCH}
    SOVERSION ${IGSTK_VERSION_MAJOR}
  )
ENDIF(IGSTK_WITH_LIBRARY_VERSION)

# Build with shared libraries
OPTION(BUILD_SHARED_LIBS "Build with shared libraries" OFF)

ENABLE_TESTING()
INCLUDE (${CMAKE_ROOT}/Modules/Dart.cmake)

SET(CXX_TEST_PATH ${EXECUTABLE_OUTPUT_PATH})

#
#  FIND ITK
#
find_package(ITK REQUIRED)
set( ITK_MIN_VERSION 4.2.0 )

set( ITK_VERSION ${ITK_VERSION_MAJOR}.${ITK_VERSION_MINOR}.${ITK_VERSION_PATCH} )

if( ${ITK_VERSION} VERSION_LESS ${ITK_MIN_VERSION} )
  message( FATAL_ERROR "This application requires ITK version ${ITK_MIN_VERSION} or later. Provided version is ${ITK_VERSION}.")
else()
  INCLUDE (${USE_ITK_FILE})
endif()

#
# FIND VTK
#
find_package(VTK REQUIRED)
set( VTK_MIN_VERSION 5.10.0 )

set( VTK_VERSION ${VTK_VERSION_MAJOR}.${VTK_VERSION_MINOR}.${VTK_VERSION_PATCH} )

if( ${VTK_VERSION} VERSION_LESS ${VTK_MIN_VERSION} )
  message( FATAL_ERROR "This application requires VTK version ${VTK_MIN_VERSION} or later. Provided version is ${VTK_VERSION}.")
else()
  INCLUDE (${USE_VTK_FILE})
endif()


# The IGSTK Library
SUBDIRS(Source)

# Create the list of include directories needed for IGSTK header files.
INCLUDE(${IGSTK_SOURCE_DIR}/igstkIncludeDirectories.cmake)

# This should be the only INCLUDE_DIRECTORIES command in the entire
# tree, except for the Utilities and Wrapping directories.  We need to
# do this in one place to make sure the order is correct.
INCLUDE_DIRECTORIES(
  ${ITK_DIR}/Utilities/zlib
  ${IGSTK_INCLUDE_DIRS_BUILD_TREE}
  ${IGSTK_INCLUDE_DIRS_BUILD_TREE_CXX}
  ${IGSTK_INCLUDE_DIRS_SYSTEM}
)

#-----------------------------------------------------------------------------
# check for header files that we need
INCLUDE (${CMAKE_ROOT}/Modules/CheckIncludeFile.cmake)
# for serial communication: termio.h or termios.h?
CHECK_INCLUDE_FILE("termios.h"       HAVE_TERMIOS_H)
CHECK_INCLUDE_FILE("termio.h"        HAVE_TERMIO_H)

# Configure a header needed by igstkSystemInformation.
CONFIGURE_FILE("${IGSTK_SOURCE_DIR}/igstkConfigure.h.in"
               "${IGSTK_BINARY_DIR}/igstkConfigure.h")

#-----------------------------------------------------------------------------
# Help other projects use IGSTK.
SET(IGSTK_LIBRARY_PATH ${LIBRARY_OUTPUT_PATH})


# Copy the UseIGSTK.cmake file to the binary tree for backward compatability.
CONFIGURE_FILE(${IGSTK_SOURCE_DIR}/UseIGSTK.cmake.in
               ${IGSTK_BINARY_DIR}/UseIGSTK.cmake COPYONLY IMMEDIATE)

#-----------------------------------------------------------------------------
# Installation vars.
# IGSTK_INSTALL_BIN_DIR          - binary dir (executables)
# IGSTK_INSTALL_LIB_DIR          - library dir (libs)
# IGSTK_INSTALL_DATA_DIR         - share dir (say, examples, data, etc)
# IGSTK_INSTALL_INCLUDE_DIR      - include dir (headers)
# IGSTK_INSTALL_PACKAGE_DIR      - package/export configuration files

IF(NOT IGSTK_INSTALL_BIN_DIR)
   SET(IGSTK_INSTALL_BIN_DIR
      ${CMAKE_INSTALL_PREFIX}/bin
   )
ENDIF(NOT IGSTK_INSTALL_BIN_DIR)

IF(NOT IGSTK_INSTALL_LIB_DIR)
   SET(IGSTK_INSTALL_LIB_DIR
      ${CMAKE_INSTALL_PREFIX}/lib/IGSTK
   )
ENDIF(NOT IGSTK_INSTALL_LIB_DIR)


IF(NOT IGSTK_INSTALL_INCLUDE_DIR)
   SET(IGSTK_INSTALL_INCLUDE_DIR
      ${CMAKE_INSTALL_PREFIX}/include/IGSTK
   )
ENDIF(NOT IGSTK_INSTALL_INCLUDE_DIR)

IF(NOT IGSTK_INSTALL_PACKAGE_DIR)
   SET(IGSTK_INSTALL_PACKAGE_DIR
      ${IGSTK_INSTALL_LIB_DIR}
#      CACHE INTERNAL ""
   )
ENDIF(NOT IGSTK_INSTALL_PACKAGE_DIR)


# Save library dependencies.
EXPORT_LIBRARY_DEPENDENCIES(${IGSTK_BINARY_DIR}/IGSTKLibraryDepends.cmake)

# Create the IGSTKConfig.cmake file containing the IGSTK configuration.
INCLUDE (${IGSTK_SOURCE_DIR}/igstkGenerateIGSTKConfig.cmake)

# Install some files.
INSTALL(FILES
   ${IGSTK_BINARY_DIR}/igstkConfigure.h
   DESTINATION ${IGSTK_INSTALL_INCLUDE_DIR}
)

INSTALL(FILES
   ${IGSTK_BINARY_DIR}/IGSTKBuildSettings.cmake
   ${IGSTK_BINARY_DIR}/IGSTKLibraryDepends.cmake
   ${IGSTK_BINARY_DIR}/UseIGSTK.cmake
   DESTINATION ${IGSTK_INSTALL_PACKAGE_DIR}
)

#-----------------------------------------------------------------------------
# Set up CPack support, to be able to distribute IGSTK
#
INCLUDE(InstallRequiredSystemLibraries)
SET(CPACK_PACKAGE_DESCRIPTION_SUMMARY "IGSTK ")
SET(CPACK_PACKAGE_VENDOR "ISC")
SET(CPACK_PACKAGE_VERSION_MAJOR "${IGSTK_VERSION_MAJOR}")
SET(CPACK_PACKAGE_VERSION_MINOR "${IGSTK_VERSION_MINOR}")
SET(CPACK_PACKAGE_VERSION_PATCH "${IGSTK_VERSION_PATCH}")
SET(CPACK_PACKAGE_INSTALL_DIRECTORY "IGSTK-${IGSTK_VERSION_MAJOR}.${IGSTK_VERSION_MINOR}")
INCLUDE(CPack)


#-----------------------------------------------------------------------------
# Add compiler flags IGSTK needs to work on this platform.  This must be
# done AFTER the call to CMAKE_EXPORT_BUILD_SETTINGS.
#-----------------------------------------------------------------------------
# ITK requires special compiler flags on some platforms.
if(CMAKE_COMPILER_IS_GNUCXX)
  set(IGSTK_REQUIRED_C_FLAGS "${IGSTK_REQUIRED_C_FLAGS} -Wall -Wno-uninitialized -Wno-unused-parameter")
  set(IGSTK_REQUIRED_CXX_FLAGS "${IGSTK_REQUIRED_CXX_FLAGS} -ftemplate-depth-50 -Wall")
  include(${IGSTK_SOURCE_DIR}/CMake/itkCheckCXXAcceptsFlags.cmake)
  itkCHECK_CXX_ACCEPTS_FLAGS("-Wno-deprecated" CXX_HAS_DEPRECATED_FLAG)
  if(CXX_HAS_DEPRECATED_FLAG)
    set(IGSTK_REQUIRED_CXX_FLAGS "${IGSTK_REQUIRED_CXX_FLAGS} -Wno-deprecated")
  endif(CXX_HAS_DEPRECATED_FLAG)
endif(CMAKE_COMPILER_IS_GNUCXX)

SET(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${IGSTK_REQUIRED_C_FLAGS}")
SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${IGSTK_REQUIRED_CXX_FLAGS}")
SET(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${IGSTK_REQUIRED_LINK_FLAGS}")
SET(CMAKE_SHARED_LINKER_FLAGS "${CMAKE_SHARED_LINKER_FLAGS} ${IGSTK_REQUIRED_LINK_FLAGS}")
SET(CMAKE_MODULE_LINKER_FLAGS "${CMAKE_MODULE_LINKER_FLAGS} ${IGSTK_REQUIRED_LINK_FLAGS}")

IF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)
  ADD_DEFINITIONS(-D_CRT_SECURE_NO_DEPRECATE -D_CRT_NONSTDC_NO_DEPRECATE -D_CRT_SECURE_NO_WARNINGS)
  ADD_DEFINITIONS(-D_SCL_SECURE_NO_DEPRECATE -D_SCL_SECURE_NO_WARNINGS)
ENDIF(MSVC_VERSION EQUAL 1400 OR MSVC_VERSION GREATER 1400)

SUBDIRS(Utilities)

OPTION(BUILD_DOCUMENTATION "Build IGSTK Documentation" OFF)
IF(BUILD_DOCUMENTATION)
  find_package(VTK)
  IF (USE_VTK_FILE)
    INCLUDE (${USE_VTK_FILE})
  ELSE (USE_VTK_FILE)
    MESSAGE( FATAL_ERROR "This application requires VTK. One of these components is missing. Please verify configuration")
  ENDIF (USE_VTK_FILE)
  SUBDIRS(Utilities/Doxygen)
ENDIF(BUILD_DOCUMENTATION)

#-----------------------------------------------------------------------------
# IGSTK uses KWStyle for checking the coding style
OPTION(IGSTK_USE_KWSTYLE "Run KWStyle in order to check for violations of the coding standard." OFF)
MARK_AS_ADVANCED(IGSTK_USE_KWSTYLE)

IF(IGSTK_USE_KWSTYLE)
  FIND_PROGRAM(KWSTYLE_EXECUTABLE
  NAMES KWStyle
  PATHS
  /usr/local/bin
  )
  MARK_AS_ADVANCED(KWSTYLE_EXECUTABLE)

  OPTION(KWSTYLE_USE_VIM_FORMAT "Set KWStyle to generate errors with a VIM-compatible format." OFF)
  OPTION(KWSTYLE_USE_MSVC_FORMAT "Set KWStyle to generate errors with a VisualStudio-compatible format." OFF)

  CONFIGURE_FILE(${IGSTK_SOURCE_DIR}/Utilities/KWStyle/IGSTK.kws.xml.in
               ${IGSTK_BINARY_DIR}/Utilities/KWStyle/IGSTK.kws.xml)

  CONFIGURE_FILE(${IGSTK_SOURCE_DIR}/Utilities/KWStyle/IGSTKFiles.txt.in
               ${IGSTK_BINARY_DIR}/Utilities/KWStyle/IGSTKFiles.txt)

  SET(KWSTYLE_ARGUMENTS -xml ${IGSTK_BINARY_DIR}/Utilities/KWStyle/IGSTK.kws.xml
                        -o ${IGSTK_SOURCE_DIR}/Utilities/KWStyle/IGSTKOverwrite.txt
                        -v
                        -D ${IGSTK_BINARY_DIR}/Utilities/KWStyle/IGSTKFiles.txt
  )

  IF(KWSTYLE_USE_VIM_FORMAT)
    SET(KWSTYLE_ARGUMENTS -vim ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_VIM_FORMAT)

  IF(KWSTYLE_USE_MSVC_FORMAT)
    SET(KWSTYLE_ARGUMENTS -msvc ${KWSTYLE_ARGUMENTS})
  ENDIF(KWSTYLE_USE_MSVC_FORMAT)

    ADD_CUSTOM_COMMAND(
    OUTPUT  ${IGSTK_BINARY_DIR}/KWStyleReport.txt
    COMMAND ${KWSTYLE_EXECUTABLE}
    ARGS    ${KWSTYLE_ARGUMENTS}
    COMMENT "Coding Style Checker"
    )
  ADD_CUSTOM_TARGET(StyleCheck DEPENDS ${IGSTK_BINARY_DIR}/KWStyleReport.txt)
ENDIF(IGSTK_USE_KWSTYLE)

# Microsoft Visual Studio 10 needs additional flag in order to compile in
# 64 Bit mode
IF(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")
  SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /bigobj")
ENDIF(CMAKE_GENERATOR STREQUAL "Visual Studio 10 Win64")

IF( WIN32 )
  SET( IGSTK_SERIAL_PORT_0 "COM1:" CACHE STRING "First serial port device name (Default COM1)")
  SET( IGSTK_SERIAL_PORT_1 "COM2:" CACHE STRING "Second serial port device name (Default COM2)")
  SET( IGSTK_SERIAL_PORT_2 "COM3:" CACHE STRING "Third serial port device name (Default COM3)")
  SET( IGSTK_SERIAL_PORT_3 "COM4:" CACHE STRING "Fourth serial port device name (Default COM4)")
  SET( IGSTK_SERIAL_PORT_4 "COM5:" CACHE STRING "Fifith serial port device name (Default COM5)")
  SET( IGSTK_SERIAL_PORT_5 "COM6:" CACHE STRING "Sixth serial port device name (Default COM6)")
  SET( IGSTK_SERIAL_PORT_6 "COM7:" CACHE STRING "Seventh serial port device name (Default COM7)")
  SET( IGSTK_SERIAL_PORT_7 "COM8:" CACHE STRING "Eighth serial port device name (Default COM7)")
ELSE (WIN32 )
  SET( IGSTK_SERIAL_PORT_0 "/dev/ttyS0" CACHE STRING "First serial port device name  (Default /dev/ttyS0)")
  SET( IGSTK_SERIAL_PORT_1 "/dev/ttyS1" CACHE STRING "Second serial port device name  (Default /dev/ttyS1)")
  SET( IGSTK_SERIAL_PORT_2 "/dev/ttyS2" CACHE STRING "Third serial port device name  (Default /dev/ttyS2)")
  SET( IGSTK_SERIAL_PORT_3 "/dev/ttyS3" CACHE STRING "Fourth serial port device name  (Default /dev/ttyS3)")
  SET( IGSTK_SERIAL_PORT_4 "/dev/ttyS4" CACHE STRING "Fifith serial port device name  (Default /dev/ttyS4)")
  SET( IGSTK_SERIAL_PORT_5 "/dev/ttyS5" CACHE STRING "Sixth serial port device name  (Default /dev/ttyS5)")
  SET( IGSTK_SERIAL_PORT_6 "/dev/ttyS6" CACHE STRING "Seventh serial port device name  (Default /dev/ttyS6)")
  SET( IGSTK_SERIAL_PORT_7 "/dev/ttyS7" CACHE STRING "Eighth serial port device name  (Default /dev/ttyS7)")
ENDIF ( WIN32 )
#----------------------------------------------------------------------------
configure_file(CMake/CTestCustom.cmake.in CTestCustom.cmake @ONLY)
