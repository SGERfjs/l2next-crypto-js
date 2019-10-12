# Copyright (c) 2019 Alexei Gaidulean
# All rights reserved.
#
# Use of this source code is governed by a MIT-style license that
# can be found in the LICENSE file.
#

#
# Global setup.
#

# CMake version policy
cmake_minimum_required(VERSION 3.1...3.14)

if(${CMAKE_VERSION} VERSION_LESS 3.12)
	cmake_policy(VERSION ${CMAKE_MAJOR_VERSION}.${CMAKE_MINOR_VERSION})
endif()

# Includes.
include(${CMAKE_ROOT}/Modules/ExternalProject.cmake)

# Project name.
project(L2NextCryptoJS)

# Set variables.
set(LIB_NAME L2NextCrypto.lib)

# Add L2NextCrypto dependency
ExternalProject_Add(L2NextCrypto
  GIT_REPOSITORY https://github.com/ThunderstormPro/l2next-crypto.git
  GIT_TAG v.0.1.1
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/L2NextCrypto
  CMAKE_ARGS -DCMAKE_INSTALL_PREFIX:PATH=<INSTALL_DIR>
  TMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/tmp/L2NextCrypto
  STAMP_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Stamp/L2NextCrypto
  DOWNLOAD_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Download/L2NextCrypto
  SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Source/L2NextCrypto
  BINARY_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Build/L2NextCrypto
  INSTALL_DIR ${CMAKE_CURRENT_SOURCE_DIR}/External/Install/L2NextCrypto
)

# Get installation directory of the library.
ExternalProject_Get_property(L2NextCrypto INSTALL_DIR)

# Get all files.
file(GLOB_RECURSE SOURCE_FILES "Source/*.cpp" "Source/*.h")

# Build a static cmake-js library sample.
add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES} ${CMAKE_JS_SRC})

# Gives our library file a .node extension without any "lib" prefix
set_target_properties(${PROJECT_NAME} PROPERTIES PREFIX "" SUFFIX ".node")

# Essential include files to build a node addon.
target_include_directories(${PROJECT_NAME} PRIVATE ${CMAKE_JS_INC})

# Include L2NextCrypto.
include_directories(${PROJECT_NAME} "${INSTALL_DIR}/include" "${INSTALL_DIR}/bin")

# Essential library files to link to a node addon.
target_link_libraries(${PROJECT_NAME} ${CMAKE_JS_LIB}) 
