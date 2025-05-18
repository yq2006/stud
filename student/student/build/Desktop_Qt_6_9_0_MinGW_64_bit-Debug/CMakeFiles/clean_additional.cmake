# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles\\student_autogen.dir\\AutogenUsed.txt"
  "CMakeFiles\\student_autogen.dir\\ParseCache.txt"
  "student_autogen"
  )
endif()
