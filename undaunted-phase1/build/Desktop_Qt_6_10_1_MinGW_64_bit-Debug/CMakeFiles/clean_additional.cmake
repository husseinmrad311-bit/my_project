# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\undaunted-phase1_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\undaunted-phase1_autogen.dir\\ParseCache.txt"
  "undaunted-phase1_autogen"
  )
endif()
