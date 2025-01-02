# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Release")
  file(REMOVE_RECURSE
  "CMakeFiles\\gobangForAssignment_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\gobangForAssignment_autogen.dir\\ParseCache.txt"
  "gobangForAssignment_autogen"
  )
endif()
