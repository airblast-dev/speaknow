# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(rightnow2)
if(${IDF_TARGET} STREQUAL "linux")
idf_build_set_property(COMPILE_OPTIONS "-fsanitize=undefined" "-fno-sanitize=shift-base" APPEND)
  idf_build_set_property(COMPILE_OPTIONS "-static-libasan" APPEND)
endif()
