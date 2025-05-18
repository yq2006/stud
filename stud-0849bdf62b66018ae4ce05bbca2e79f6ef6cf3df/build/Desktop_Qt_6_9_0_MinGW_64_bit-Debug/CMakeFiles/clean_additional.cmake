# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\stud_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\stud_autogen.dir\\ParseCache.txt"
  "stud_autogen"
  )
endif()
