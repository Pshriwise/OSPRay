## ======================================================================== ##
## Copyright 2009-2016 Intel Corporation                                    ##
##                                                                          ##
## Licensed under the Apache License, Version 2.0 (the "License");          ##
## you may not use this file except in compliance with the License.         ##
## You may obtain a copy of the License at                                  ##
##                                                                          ##
##     http://www.apache.org/licenses/LICENSE-2.0                           ##
##                                                                          ##
## Unless required by applicable law or agreed to in writing, software      ##
## distributed under the License is distributed on an "AS IS" BASIS,        ##
## WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. ##
## See the License for the specific language governing permissions and      ##
## limitations under the License.                                           ##
## ======================================================================== ##

PROJECT(OSPRay)

CMAKE_MINIMUM_REQUIRED(VERSION 2.8)
CMAKE_POLICY(SET CMP0003 NEW) # since 2.6
IF(POLICY CMP0015)
  CMAKE_POLICY(SET CMP0015 NEW) # since 2.8.1
ENDIF()
IF(POLICY CMP0042)
  CMAKE_POLICY(SET CMP0042 NEW) # since 3.0
ENDIF()
IF(POLICY CMP0054)
  CMAKE_POLICY(SET CMP0054 OLD) # since 3.1
ENDIF()

SET(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${PROJECT_SOURCE_DIR}/cmake)

##############################################################
# CMake modules and macro files
##############################################################

INCLUDE(ospray_macros)
INCLUDE(ospray_options)
INCLUDE(ispc)
INCLUDE(mpi)

IF(NOT WIN32)
  INCLUDE(cmake/doxygen.cmake)
ENDIF()

##############################################################
# the OSPRay 'common' library
##############################################################

OSPRAY_ADD_SUBDIRECTORY(ospcommon)

##############################################################
# the OSPRay library
##############################################################

OSPRAY_ADD_SUBDIRECTORY(ospray)

##############################################################
# OSPRay modules
##############################################################

OPTION(OSPRAY_ENABLE_MODULES "Enable the 'modules' subtree in the build." ON)
MARK_AS_ADVANCED(OSPRAY_ENABLE_MODULES)

IF (OSPRAY_ENABLE_MODULES)
  OSPRAY_ADD_SUBDIRECTORY(modules)
ENDIF()

##############################################################
# OSPRay sample apps; at the end: they may need modules
##############################################################

OPTION(OSPRAY_ENABLE_APPS "Enable the 'apps' subtree in the build." ON)
MARK_AS_ADVANCED(OSPRAY_ENABLE_APPS)

IF (OSPRAY_ENABLE_APPS)
  # NOTE(jda) - Don't use OSPRAY_ADD_SUBDIRECTORY() macro as we don't have any
  #             KNC code in the apps subdirectory to compile.
  ADD_SUBDIRECTORY(apps)
ENDIF()

# create a configure file that both ospray and ispc can read the cmake config
# from needs to be at the end, after all cache variables have been set
CONFIGURE_FILE(ospray/common/OSPConfig.h.in OSPConfig.h)
INSTALL(FILES ${CMAKE_BINARY_DIR}/OSPConfig.h
  DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/ospray
  COMPONENT devel
)

# has to be last
INCLUDE(CPack)
