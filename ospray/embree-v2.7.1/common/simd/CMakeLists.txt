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

IF (__XEON__)
  ADD_LIBRARY(simd STATIC sse.cpp)
  SET_PROPERTY(TARGET simd PROPERTY FOLDER common)
  SET_TARGET_PROPERTIES(simd PROPERTIES COMPILE_FLAGS "-DDONT_WARN_INCLUDE_EMBREE_INTERNALS=1")
ELSE ()
  INCLUDE(icc_xeonphi)
  ADD_LIBRARY(simd_xeonphi STATIC vboolf16_avx512.cpp)
  SET_TARGET_PROPERTIES(simd_xeonphi PROPERTIES COMPILE_FLAGS "-DDONT_WARN_INCLUDE_EMBREE_INTERNALS=1")
ENDIF ()

