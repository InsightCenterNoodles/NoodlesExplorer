# Additional clean files
cmake_minimum_required(VERSION 3.16)

if("${CONFIG}" STREQUAL "" OR "${CONFIG}" STREQUAL "Debug")
  file(REMOVE_RECURSE
  "CMakeFiles/NoodlesExplorer_autogen.dir/AutogenUsed.txt"
  "CMakeFiles/NoodlesExplorer_autogen.dir/ParseCache.txt"
  "NoodlesExplorer_autogen"
  "_deps/noodles-build/CMakeFiles/noodles_autogen.dir/AutogenUsed.txt"
  "_deps/noodles-build/CMakeFiles/noodles_autogen.dir/ParseCache.txt"
  "_deps/noodles-build/noodles_autogen"
  )
endif()
