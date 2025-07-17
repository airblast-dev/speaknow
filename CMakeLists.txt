# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.16)

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(rightnow2)

if(${PROJECT_NAME} STREQUAL "unit_test_test")
  if(${IDF_TARGET} STREQUAL "linux")
    idf_build_set_property(COMPILE_OPTIONS "-fsanitize=undefined" "-fsanitize=shadow-call-stack" "-fsanitize=leak" "-fsanitize=vla-bound" "-fsanitize=null" "-fsanitize=bounds-strict" "-fsanitize=alignment" "-fsanitize=enum" "-fstack-protector" APPEND)
  else()
    idf_build_set_property(COMPILE_OPTIONS "-fsanitize=undefined" "-fno-sanitize=shift-base" "-fsanitize=shadow-call-stack" "-fsanitize=leak" "-fsanitize=vla-bound" "-fsanitize=null" "-fsanitize=bounds-strict" "-fsanitize=alignment" "-fsanitize=enum" "-fstack-protector" APPEND)
  endif()
endif()
